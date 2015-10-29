#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
class G4LogicalVolume;
class G4VPhysicalVolume;

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4UIcommand.hh"
#include "G4NistManager.hh"

#include "SensitiveDetector.hh"
#include "Settings.hh"

#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <string>
#include <vector>
#include <stdlib.h>

//#include <fstream>
//#include <stdlib.h>

using namespace std;

class DetectorConstruction : public G4VUserDetectorConstruction{
public:

  DetectorConstruction();
  DetectorConstruction(Settings* set){
    fset= set;
  };
  ~DetectorConstruction();

  G4VPhysicalVolume* Construct();

private:
  Settings* fset;

};

#endif

