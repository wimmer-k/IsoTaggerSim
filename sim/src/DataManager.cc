#include "DataManager.hh"
ClassImp(HodoRaw)

using namespace std;
using namespace CLHEP;

DataManager::DataManager(){
  cout << "bla" << endl;
}
DataManager::DataManager(const char* filename, int nevents){
  fnevents = nevents;
  ffile = new TFile(filename,"RECREATE");
  ffile->cd();
  fesim.clear();
  fhodoraw = new HodoRaw;
  ftree = new TTree("str","simulation data");
  ftree->Branch("HodoRaw",&fhodoraw,32000);
  ftree->BranchRef();
}
DataManager::~DataManager(){
  this->Close();
  delete ftree;
  delete ffile;
  delete fhodoraw;
}
void DataManager::SetEsim(double e){
  fesim.push_back(e);
}
void DataManager::SetPossim(double x, double y, double z){
  fpossim = TVector3(x,y,z);
}
void DataManager::FillTree(HodoRaw *inhodo){
  fhodoraw->Clear();
  fhodoraw = inhodo;
  fhodoraw->SetESim(fesim);
  fhodoraw->SetPosSim(fpossim);
  //fhodoraw->Print();
  ftree->Fill();
  fesim.clear();
}
void DataManager::Clear(const Option_t*){
  fhodoraw->Clear();
}
void DataManager::Close(){
  ffile->cd();
  ftree->Write();
  ffile->Close();
}



