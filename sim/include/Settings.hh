#ifndef Settings_h
#define Settings_h 1

#include <iostream>
#include <fstream>
#include <string>

class Settings;
#include "TSystem.h"
#include "TEnv.h"
#include "TObject.h"
#include "TMath.h"
using namespace std;

#define deg2rad TMath::Pi()/180.;
#define rad2deg 180./TMath::Pi();


class Settings{
public:
  Settings();//default ctor
  Settings(const char*);
  ~Settings();

  void ReadSettings();
  void PrintSettings();
  void SetFile(const char* filename){fInputFile = filename;}
  const string InputFile(){return fInputFile;}

  int VLevel(){return fVerboseLevel;}
 
  double GammaEnergy(){return fGammaEnergy;}
  double GammaWhite(int i){return fGammaWhite[i];}
  double GammaWhiteFrom(){return fGammaWhite[0];}
  double GammaWhiteTo(){return fGammaWhite[1];}
  const char* GammaLevelFile(){return fGammaLevelFile.c_str();}
  int GammaDirection(){return fGammaDirection;}
  double GammaDirectionTheta(){return fGammaTheta;}
  double GammaDirectionPhi(){return fGammaPhi;}

  int ReferencePosition(){return fReferencePosition;}
  double SourcePositionX(){return fPosX;} 
  double SourcePositionY(){return fPosY;} 
  double SourcePositionZ(){return fPosZ;} 

  const char* PosHistFile(){return fPosHistFile.c_str();}
  const char* PosHistName(){return fPosHistName.c_str();}

  int ChamberON(){return fChamberON;}
  double ChamberHeight(){return fChamberHeight;}
  double ChamberWidth(){return fChamberWidth;}
  double ChamberLength(){return fChamberLength;}
  double ChamberThickness(){return fChamberThick;}
  double ChamberPositionZ(){return fChamberPosZ;} 

  double PlateHeight(){return fPlateHeight;}
  double PlateWidth(){return fPlateWidth;}
  double PlateThickness(){return fPlateThick;}
  double PlateDistance(){return fPlateDist;}
  double PlatePositionX(){return fPlatePosX;} 
  double PlatePositionY(){return fPlatePosY;} 
  double CrystalDepth(){return fCrystZ;}
  double CrystalWidth(){return fCrystY;}
  double CrystalHeight(){return fCrystX;}

protected:
  int fVerboseLevel;
  string fInputFile;
  double fGammaEnergy;
  double fGammaWhite[2];
  string fGammaLevelFile;
  int fGammaDirection;
  double fGammaTheta;
  double fGammaPhi;
  int fReferencePosition;
  double fPosX;
  double fPosY;
  double fPosZ;
  string fPosHistFile;
  string fPosHistName;
  int fChamberON;
  double fChamberHeight;
  double fChamberWidth;
  double fChamberLength;
  double fChamberThick;
  double fChamberPosZ;
  double fPlateHeight;
  double fPlateWidth;
  double fPlateThick;
  double fPlateDist;
  double fPlatePosX;
  double fPlatePosY;
  double fCrystZ;
  double fCrystY;
  double fCrystX;

};
#endif
