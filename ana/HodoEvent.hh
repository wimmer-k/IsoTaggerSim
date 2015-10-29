#ifndef __HodoEvent_HH
#define __HodoEvent_HH

#include <iostream>
#include <vector>
#include "TObject.h"
#include "TVector3.h"
using namespace std;

class HodoRaw : public TObject {
public:
  HodoRaw(){
    Clear();
  }
  ~HodoRaw(){
    //this->Clear();
  }  
  void Clear(){
    fEtot = 0;
    fEsim.clear();
    fE.clear();
    fE.resize(32);
    fmult = 0;
    fPossim.SetXYZ(-500,0,0);
  }
  void SetESim(vector<double> esim){
    fEsim = esim;
  }
  void SetPosSim(double x, double y, double z){
    fPossim = TVector3(x,y,z);
  }
  void SetPosSim(TVector3 pos){
    fPossim = pos;
  }
  void SetETot(double etot){
    fEtot = etot;
  }
  void SetE(double e, int ch){
    if(ch<0||ch>31){
      cout << "invalid channel " << ch << " couldn't set energy " << e << "!" << endl;
    }
    fE[ch] = e;
    fmult++;
  }
  void SetMult(int m){
    fmult = m;
  }

  vector<double> GetESim(){
    return fEsim;
  }
  double GetESim(int i){
    return fEsim[i];
  }
  TVector3 GetPosSim(){
    return fPossim;
  }
  double GetETot(){
    return fEtot;
  }
  double GetE(int ch){
    if(ch<0||ch>31){
      cout << "invalid channel " << ch << " couldn't get energy!" << endl;
    }
    return fE[ch];
  }
  vector<double> GetE(){
    return fE;
  }
  int GetMult(){
    return fmult;
  }
  void Print(){
    //cout << "simulated " << fEsim << " at x = " << fPossim.X() << " y = " << fPossim.Y() << " z = " << fPossim.Z() << " detected " << fEtot << " mult " << fmult << endl;
    cout << "simulated at x = " << fPossim.X() << " y = " << fPossim.Y() << " z = " << fPossim.Z() << endl;
    for(UShort_t i=0;i<fEsim.size();i++){
      cout << " event " << i << ":\t" << fEsim[i] << " keV" << endl;
    }
    cout << "detected total " << fEtot << " mult " << fmult << endl;
    for(int i=0;i<32;i++){
      if(fE[i]>0)
	cout << "crystal " << i << " energy " << fE[i] << endl;
    }

  }
protected:
  vector<double> fEsim;
  double fEtot;
  TVector3 fPossim;
  vector<double> fE;
  int fmult;

  ClassDef(HodoRaw, 1);
};
#endif
