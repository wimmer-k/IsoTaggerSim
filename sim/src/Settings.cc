#include "Settings.hh"

using namespace std;

Settings::Settings(){
  cout << "not filename " << endl;
}
Settings::Settings(const char* filename){
  SetFile( filename );
  ReadSettings();
  if(fVerboseLevel>1)
    PrintSettings();
}
Settings::~Settings(){
  cout << "destructor" << endl;
}
void Settings::ReadSettings(){
  //char* defaultfile = (char*)"~/analysis/settings/nocal.dat";

  TEnv *set = new TEnv(fInputFile.data()); 
  fVerboseLevel = set->GetValue("VerboseLevel",0);

  fGammaEnergy = set->GetValue("Gamma.Energy",1000.);
  fGammaWhite[0] = set->GetValue("Gamma.White.From",-1.0);
  fGammaWhite[1] = set->GetValue("Gamma.White.To",-1.0);
  fGammaLevelFile = set->GetValue("Gamma.Level.File","nofile");
  fGammaDirection = set->GetValue("Gamma.Direction",0);
  fGammaTheta = set->GetValue("Gamma.Direction.Theta",0.);
  fGammaPhi = set->GetValue("Gamma.Direction.Phi",0.);
  
  fReferencePosition = set->GetValue("Reference.Position",0);
  
  fPosX = set->GetValue("Source.Position.X",0.0);
  fPosY = set->GetValue("Source.Position.Y",0.0);
  fPosZ = set->GetValue("Source.Position.Z",0.0);

  
  fPlateHeight = set->GetValue("Plate.Height",762.0);
  fPlateWidth = set->GetValue("Plate.Width",368.3);
  fPlateThick = set->GetValue("Plate.Thickness",6.35);
  fPlateDist = set->GetValue("Plate.Distance",105.3);
  fPlatePosX = set->GetValue("Plate.Position.X",0.0);
  fPlatePosY = set->GetValue("Plate.Position.Y",0.0);

  fCrystZ = set->GetValue("Crystal.Depth",51.0);
  fCrystY = set->GetValue("Crystal.Width",82.5);
  fCrystX = set->GetValue("Crystal.Height",82.5);

  //numbers from drawing in mm
  fChamberON = set->GetValue("Chamber.ON",1); // 1 on, 0 off
  fChamberHeight = set->GetValue("Chamber.Height",1140.75); //x = up/down
  fChamberWidth = set->GetValue("Chamber.Width",579.15); //y
  fChamberLength = set->GetValue("Chamber.Length",2474.55); //z
  fChamberThick = set->GetValue("Chamber.Thickness",100); //10 cm wall thickness
  fChamberPosZ = set->GetValue("Chamber.Position.Z",877.5);

  fPosHistFile = set->GetValue("Position.Hist.File","nofile");
  fPosHistName = set->GetValue("Position.Hist.Name","nofile");

  if(fReferencePosition==1){
    fPosX-= fPlatePosX;
    fPosY-= fPlatePosY;
  }

  //convert to S800 coordinate system. 
  //focal plane:
  // x down
  // z forward
  // y left  
  fPosY = -fPosY;
  fPosZ = -fPosZ;
}
void Settings::PrintSettings(){
 cout << "VerboseLevel\t" << fVerboseLevel << endl;
 
 cout << "Gamma.Energy\t" << fGammaEnergy << endl;
 cout << "Gamma.Energy.White.0\t" << fGammaWhite[0] << endl;
 cout << "Gamma.Energy.White.1\t" << fGammaWhite[1] << endl;
 cout << "Gamma.Level.File\t" << fGammaLevelFile << endl;
 cout << "Gamma.Direction\t" << fGammaDirection << endl;
 cout << "Gamma.Direction.Theta\t" << fGammaTheta << endl;
 cout << "Gamma.Direction.Phi\t" << fGammaPhi << endl;
 
 cout << "Reference.Position\t" << fReferencePosition << endl;
 
 if(fReferencePosition==1){
   cout << "entered cooredinates:" << endl;
   cout << "Source.Position.X\t" << fPosX+fPlatePosX << endl; 
   cout << "Source.Position.Y\t" << fPosY+fPlatePosY << endl;   
 }
 if(fReferencePosition!=10){
   cout << "Source.Position.X\t" << fPosX << endl; 
   cout << "Source.Position.Y\t" << fPosY << endl; 
   cout << "Source.Position.Z\t" << fPosZ << endl; 
 }
 else{
  cout << "Position.Hist.File\t" << fPosHistFile << endl;
  cout << "Position.Hist.Name\t" << fPosHistName << endl;
 }

 cout << "Chamber.ON\t" << fChamberON << endl;
 cout << "Chamber.Height\t" << fChamberHeight << endl;
 cout << "Chamber.Width\t" << fChamberWidth << endl;
 cout << "Chamber.Length\t" << fChamberLength << endl;
 cout << "Chamber.Thickness\t" << fChamberThick << endl;
 cout << "Chamber.Position.Z\t" << fChamberPosZ << endl; 

 cout << "Plate.Height\t" << fPlateHeight << endl;
 cout << "Plate.Width\t" << fPlateWidth << endl;
 cout << "Plate.Thickness\t" << fPlateThick << endl;
 cout << "Plate.Distance\t" << fPlateDist << endl;
 cout << "Plate.Position.X\t" << fPlatePosX << endl; 
 cout << "Plate.Position.Y\t" << fPlatePosY << endl; 

 cout << "Crystal.Depth\t" << fCrystZ << endl;
 cout << "Crystal.Width\t" << fCrystY << endl;
 cout << "Crystal.Height\t" << fCrystX << endl;
}
