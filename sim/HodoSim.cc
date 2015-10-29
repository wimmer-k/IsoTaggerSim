#include "TROOT.h"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
//#include "G4UIterminal.hh"
//#include "G4UItcsh.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#include "VisManager.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Settings.hh"
#include "DataManager.hh"
#include "HodoEvent.hh"
#include "CommandLineInterface.hh"

using namespace std;
using namespace CLHEP;

int main(int argc,char** argv)
{
  char* rootFile = NULL;
  char* setFile = NULL;
  int nevents = 0;
  bool vis = false;
  CommandLineInterface* interface = new CommandLineInterface();
  interface->Add("-n", "number of events", &nevents);
  interface->Add("-o", "output root file", &rootFile);
  interface->Add("-s", "settings file", &setFile);
  interface->Add("-vis", "switch on visualization", &vis);

  interface->CheckFlags(argc, argv);
  
  if(setFile == NULL){
    cout << "need settings file" << endl;
    cout << "using default: /home/wimmer/hodo/sim/set.dat" << endl;
    setFile = "/home/wimmer/hodo/sim/set.dat";
  }
  if(nevents<1 && vis ==false){
    cout << "simulating 0 events" << endl;
    return 0;
  }

  if(rootFile == NULL && vis==false){
    cout << "No output ROOT file given " << endl;
    return 2;
  }
  if(rootFile == NULL && vis==true){
    rootFile = (char*)"vis.root";
  }

  Settings* set = new Settings(setFile);
  
  DataManager* data = new DataManager(rootFile,nevents);

  // choose the Random engine
  HepRandom::setTheEngine(new RanecuEngine);
  G4long seed;
  seed = time(0);
  HepRandom::setTheSeed(seed);
  // Run manager
  G4RunManager * runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  DetectorConstruction* detector = new DetectorConstruction(set);
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);
   
  // User Action classes
  //
  G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(set, data);
  runManager->SetUserAction(gen_action);

  G4UserEventAction* event_action = new EventAction(data);
  runManager->SetUserAction(event_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();
  

  if(vis){
#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
#endif        
    // Get the pointer to the User Interface manager
    //
    G4UImanager * UImanager = G4UImanager::GetUIpointer();  
#ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
      if (ui->IsGUI())
	UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
#endif
    data->Close();
#ifdef G4VIS_USE
    delete visManager;
#endif     
    if(runManager!=NULL)
      delete runManager;
    return 0;
  }
  runManager->BeamOn(nevents);
  data->Close();
  


  delete runManager;
  return 0;

}

