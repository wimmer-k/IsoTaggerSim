/***********************************************************************
 *
 *    sensitive detector for detector read-out of hodoscope
 *
 ***********************************************************************/

#include "SensitiveDetector.hh"

using namespace std;
SensitiveDetector::SensitiveDetector(G4String name)
  : G4VSensitiveDetector(name)
{
  collectionName.insert(name);
}

SensitiveDetector::~SensitiveDetector(){;}

void SensitiveDetector::Initialize(G4HCofThisEvent* evnt){
  //cout << __PRETTY_FUNCTION__ << "\t" << HC << "\t"<< name << endl;
  //cout << __PRETTY_FUNCTION__ << "\t" << SensitiveDetectorName <<"\t";
  static G4int HCID;
  HCID = G4SDManager::GetSDMpointer()->GetCollectionID(SensitiveDetectorName);
  //cout << "HCID " << HCID << endl;
  hitsCollection = new HitsCollection(SensitiveDetectorName,SensitiveDetectorName);

  evnt->AddHitsCollection(HCID, hitsCollection);
  //cout << "added " << endl;
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*){
  //cout << __PRETTY_FUNCTION__ << endl;

  G4SDManager* SDman = G4SDManager::GetSDMpointer();


  G4double edep = aStep->GetTotalEnergyDeposit();
  //cout << "edep " << edep << endl;
  if(edep==0)
    return true;

  HodoHit* newHit = new HodoHit();

  newHit->SetTrackID(aStep->GetTrack()->GetTrackID());
  newHit->SetParentID(aStep->GetTrack()->GetParentID());
  newHit->SetEdep(edep);

  G4StepPoint* stepPoint;
  if(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Transportation")
    stepPoint = aStep->GetPreStepPoint();
  else
    stepPoint = aStep->GetPostStepPoint();

  G4ThreeVector position = stepPoint->GetPosition();
  
  newHit->SetPos(position);
  
  G4TouchableHandle theTouchable = stepPoint->GetTouchableHandle();

  G4ThreeVector det_position = theTouchable->GetHistory()->GetTopTransform().TransformPoint(position);
  newHit->SetPosInDet(det_position);

  //check which HitCollectionID this is
  //cout << "checking collection id entries " <<hitsCollection->entries() << endl;
  int CollectionID = SDman->GetCollectionID(hitsCollection);
  //cout << "collection id " << CollectionID << endl;
  // CollectionID -= SDman->GetCollectionID("crystal_0");
  
  newHit->SetHodoID(CollectionID);
 
  //cout<<"CollectionID = "<<CollectionID<<endl;


  newHit->SetTrack(aStep->GetTrack());
  newHit->SetProcName(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
  newHit->SetStep(aStep->GetTrack()->GetCurrentStepNumber());
  
  if( aStep->GetTrack()->GetNextVolume() != 0 ) 
    { 
      newHit->SetVolume(aStep->GetTrack()->GetVolume()->GetName());
    } 
  else 
    {
      newHit->SetVolume("OutOfWorld");
    }

  hitsCollection->insert(newHit);
  

  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{;}







