/*************************************************************************
 * 
 *  fill histogramms at the end of an event
 * 
 *************************************************************************/

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"

class EventAction;

#include "HodoHit.hh"
#include "SensitiveDetector.hh"
#include "DataManager.hh"


class EventAction : public G4UserEventAction
{
public:
  EventAction(DataManager* data);
  ~EventAction();

public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
        
private:
  DataManager* fdata;

};

#endif

    
