#ifndef DataManager_h
#define DataManager_h 1

#include "G4UnitsTable.hh"
#include "globals.hh"

class DataManager;

#include <TROOT.h>
#include <TObject.h>
#include <TFile.h>
#include <TTree.h>

#include "HodoEvent.hh"

class DataManager{
public:
  DataManager();
  DataManager(const char* filename, int nevents);
  ~DataManager();
  void Init(const char* filename);
  void SetEsim(double e);
  void SetPossim(double x, double y, double z);
  void FillTree(HodoRaw* inhodo);
  void Clear(const Option_t* option="");
  void Close();
  int GetNevents(){return fnevents;};

private:
  int fnevents;
  HodoRaw* fhodoraw;
  vector<double> fesim;
  TVector3 fpossim;
  TTree* ftree;
  TFile* ffile;
};

#endif

