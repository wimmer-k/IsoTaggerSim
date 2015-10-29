/*************************************************************************
 * 
 *  fill histogramms at the end of an event
 * 
 *************************************************************************/

#include "EventAction.hh"

using namespace std;
using namespace CLHEP;

EventAction::EventAction(DataManager* data){
  // store the pointer to the data class locally, in order to be able to acess all information later
  fdata = data;
}
EventAction::~EventAction(){
}
void EventAction::BeginOfEventAction(const G4Event*){
  //cout << __PRETTY_FUNCTION__  << endl;
}
void EventAction::EndOfEventAction(const G4Event* evt){
  //cout << __PRETTY_FUNCTION__  << endl;

  // create a new hodoraw for storing the data
  HodoRaw *hodo = new HodoRaw();
  // make sure it is empty (not really needed)
  hodo->Clear();

  // temporary storage for all the energies
  G4double en[32];
  G4double etot =0;
  
  // get the hits collection (HC) of the current event
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();

  // loop over all 32 crystals of the hodoscope
  for(int i=0;i<32;i++){
    // reset the energy of the current crystal
    en[i] =0;

    // get all hits in this current crystal
    HitsCollection* collection = (HitsCollection*)(HCE->GetHC(i));
    
    // loop over all the hits in this crystal
    for(int j=0;j<collection->entries();j++){
      // add the energy to the energy of the current cyrstal (en[i]) and the total energy (etot)
      //cout << (*collection)[j]->GetEdep()*MeV << "\t"<< (*collection)[j]->GetEdep()*keV << "\t"<< (*collection)[j]->GetEdep() << endl;
      en[i] += (*collection)[j]->GetEdep()*1000;
      etot+=(*collection)[j]->GetEdep()*1000;
    }	       
    //if the energy in this crystal (en[i]) is >0, i.e. the crystal had at least one hit, then set the energy in the hodoraw object
    if(en[i]>0){
      //cout << "detected crystal " << i << " energy " << en[i] << endl;
      hodo->SetE(en[i], i);
    }
  }//loop over all crystals

  // if the total energy (etot) is >0, i.e. at least one of the crystals was hit, set the total energy in the hodoraw object
  if(etot>0){
    //cout << "detected etot " << etot << endl;
    hodo->SetETot(etot);
  }

  // fill the tree in the datamanager object, this saves the event to the ouput file
  fdata->FillTree(hodo);


  //status output:
  // get current event number
  G4int event_id = evt->GetEventID();
  //cout << " ^^^^ " << event_id << endl;
  // output every 10000 events
  if(event_id%10000 ==0){ 
    cout << event_id <<"/"<<fdata->GetNevents() << " done..." << endl;
  }
  

}
