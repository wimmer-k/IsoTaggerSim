#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "DataManager.hh"
#include "Settings.hh"
#include "TFile.h"
#include "TH2F.h"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
public:
  PrimaryGeneratorAction(Settings* set, DataManager* data);
  ~PrimaryGeneratorAction();
  void GeneratePrimaries(G4Event* anEvent);
  void GenerateLevelScheme(const char* file);
  
private:
  G4ParticleGun* particleGun;
  DataManager* fdata;
  Settings* fset;
  TH2F* fposdistribution;
  vector<double> fenergy;
  vector<double> ffeed;
  vector<int> fdcto;
  int fnlevel;

};

#endif


