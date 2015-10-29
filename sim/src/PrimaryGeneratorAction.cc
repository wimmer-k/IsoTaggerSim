#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4Alpha.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"

#include "Randomize.hh"

using namespace std;
using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction(Settings* set, DataManager* data){
  // store the pointer to the data class locally, in order to be able to acess all information later
  fdata = data;
  fset = set;
  TFile *input = new TFile(fset->PosHistFile());
  fposdistribution = (TH2F*)input->Get(fset->PosHistName());

  // in each event one gamma ray is emitted
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  // definition of the particle type
  particleGun->SetParticleDefinition(G4Gamma::GammaDefinition());

  // set the source position
  particleGun->SetParticlePosition(G4ThreeVector(fset->SourcePositionX()*mm, fset->SourcePositionY()*mm, fset->SourcePositionZ()*mm));
  fdata->SetPossim(fset->SourcePositionX()*mm, fset->SourcePositionY()*mm, fset->SourcePositionZ()*mm);
  if(fset->GammaEnergy()*keV >0){
    if(fset->VLevel()>3)
      cout << fset->GammaEnergy()*eV << "\t" << fset->GammaEnergy() << "\t" << fset->GammaEnergy()*keV << "\t" << fset->GammaEnergy()*MeV << endl;
    fnlevel =1;
    fenergy.resize(fnlevel);
    ffeed.resize(fnlevel);
    fdcto.resize(fnlevel);
    fenergy[0] = fset->GammaEnergy();
    ffeed[0] = 1;
    fdcto[0] = -1;
  }
  else{
    GenerateLevelScheme(fset->GammaLevelFile());
  }
  
}
PrimaryGeneratorAction::~PrimaryGeneratorAction(){
  delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
  double x,y;
  if(fset->ReferencePosition()==10){
    fposdistribution->GetRandom2(x,y);
  }
  else{
    x = fset->SourcePositionX()*mm;
    y = fset->SourcePositionY()*mm;
  }
  particleGun->SetParticlePosition(G4ThreeVector(x, y, fset->SourcePositionZ()*mm));
  fdata->SetPossim(x, y, fset->SourcePositionZ()*mm);

  double tmp;
  
  tmp = RandFlat::shoot(0.0,1.0);
  //cout << "decay " << tmp << endl;
  for(int l=0;l<fnlevel;l++){
    if(fset->VLevel()>0)
      cout << "tmp " << tmp << " ffeed["<<l<<"] " << ffeed[l] << endl;
    if(tmp<ffeed[l]){
      if(fset->VLevel()>0)
	cout << "populating level " << l  << " energy " << fenergy[l]<< endl;
      int current = l;
      //generate gamma with energy
      if(fset->GammaWhiteTo()>0){
	fenergy[l] = RandFlat::shoot(fset->GammaWhiteFrom(),fset->GammaWhiteTo());
      }
      particleGun->SetParticleEnergy(fenergy[l]*keV);
      fdata->SetEsim(fenergy[l]*MeV);
      //----------set the particle direction
      // create a vector
      G4ThreeVector v(0.0,0.0,1.0);
      double theta = fset->GammaDirectionTheta()*rad2deg;
      double phi = fset->GammaDirectionPhi()*rad2deg;
      if(fset->GammaDirection()!=2){
	// random cos(theta)  = -1..1
	theta = RandFlat::shoot(-1,1);
	// theta = random in 0..180 degree
	theta = acos(theta)*radian;
	// phi is random between -pi..pi = -180..180 degree
	phi = RandFlat::shoot(-M_PI, M_PI)*radian;
	
      }
      // set the vectors theta and phi. length doesn't matter only direction
      v.setTheta(theta);
      v.setPhi(phi);
      // set the particle gun in the direction
      particleGun->SetParticleMomentumDirection(v);
      particleGun->GeneratePrimaryVertex(anEvent);
     
      while(fdcto[current]>-1){
	if(fset->VLevel()>0)
	  cout << " feeding level " << fdcto[current] << " energy " << fenergy[fdcto[current]]<< endl;
	current = fdcto[current];
	particleGun->SetParticleEnergy(fenergy[current]*keV);
	fdata->SetEsim(fenergy[current]*MeV);
	//----------set the particle direction
	// create a vector
	G4ThreeVector v(0.0,0.0,1.0);
	double theta = fset->GammaDirectionTheta()*rad2deg;
	double phi = fset->GammaDirectionPhi()*rad2deg;
	if(fset->GammaDirection()!=2){
	  // random cos(theta)  = -1..1
	  theta = RandFlat::shoot(-1,1);
	  // theta = random in 0..180 degree
	  theta = acos(theta)*radian;
	  // phi is random between -pi..pi = -180..180 degree
	  phi = RandFlat::shoot(-M_PI, M_PI)*radian;
	  
	}
	// set the vectors theta and phi. length doesn't matter only direction
	v.setTheta(theta);
	v.setPhi(phi);
	// set the particle gun in the direction
	particleGun->SetParticleMomentumDirection(v);
	particleGun->GeneratePrimaryVertex(anEvent);
	
	if(fdcto[current]==-1)
	  break;
      }
      
      break;
    }
    
  }

}

void PrimaryGeneratorAction::GenerateLevelScheme(const char* levelfile){
  ifstream file(levelfile);
  if(file.bad()){
    cerr<<"Unable to open "<<file<<"!\nexiting ... \n";
    exit(2);
  }
  file.ignore(1000, '\n');
  file >> fnlevel;
  cout << " number of levels " << fnlevel << endl;
  fenergy.resize(fnlevel);
  ffeed.resize(fnlevel);
  fdcto.resize(fnlevel);
  file.ignore(1000, '\n');
  file.ignore(1000, '\n');
  double feedtot =0;
  for(int i=0;i<fnlevel;i++){
    file >> fenergy[i] >> ffeed[i] >>fdcto[i];
    file.ignore(1000, '\n');
    if(ffeed[i]>0)
      feedtot+=ffeed[i];
    
  }
  for(int i=0;i<fnlevel;i++){
    ffeed[i]/=feedtot;
    if(i>0&&ffeed[i]>0)
      ffeed[i]+=ffeed[i-1];
    cout << "level " << i << "\t" << fenergy[i] << "\t" <<  ffeed[i] << "\t" <<  fdcto[i] << endl;
  }

}
