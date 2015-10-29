#include "DetectorConstruction.hh"

using namespace std;

DetectorConstruction::DetectorConstruction(){
}

DetectorConstruction::~DetectorConstruction(){
}

G4VPhysicalVolume* DetectorConstruction::Construct(){

  //------------------------------------------------------ materials

  G4NistManager* NistManager = G4NistManager::Instance();

  //creating materials
  //G4double aldensity = 2.700*g/cm3;
  //G4double a = 26.98*g/mole;
  //G4Material* Al = new G4Material("Aluminum", 13., a, aldensity);
  // G4Element* Cs  = NistManager->FindOrBuildElement("Cs");
  // G4Element* I  = NistManager->FindOrBuildElement("I");
  // G4double csidensity = 4.51*g/cm3;
  // G4Material* CsI = new G4Material("CsI", csidensity, 2);
  // CsI->AddElement(Cs, 0.5);
  // CsI->AddElement(I, 0.5);

  G4Material* Al = NistManager->FindOrBuildMaterial("G4_Al");
  G4Material* CsI = NistManager->FindOrBuildMaterial("G4_CESIUM_IODIDE");

  //creating air for the world volume, this is just a dummy
  G4Element* O  = NistManager->FindOrBuildElement("O");
  G4Element* N  = NistManager->FindOrBuildElement("N");

  G4double density = 1.290*mg/cm3;
  G4Material* Air = new G4Material("Air", density, 2);
  Air->AddElement(N, 0.7);
  Air->AddElement(O, 0.3);


  //stainless steel from
  // http://hypernews.slac.stanford.edu/HyperNews/geant4/get/geometry/915/1.html
  G4Element* C  = NistManager->FindOrBuildElement("C");
  G4Element* Si  = NistManager->FindOrBuildElement("Si");
  G4Element* Cr  = NistManager->FindOrBuildElement("Cr");
  G4Element* Mn  = NistManager->FindOrBuildElement("Mn");
  G4Element* Fe  = NistManager->FindOrBuildElement("Fe");
  G4Element* Ni  = NistManager->FindOrBuildElement("Ni");

  density = 8.06*g/cm3;
  G4Material* Steel = new G4Material("Steel", density, 6);
  Steel->AddElement(C,  0.001);
  Steel->AddElement(Si, 0.007);
  Steel->AddElement(Cr, 0.18);
  Steel->AddElement(Mn, 0.01);
  Steel->AddElement(Fe, 0.712);
  Steel->AddElement(Ni, 0.09);

  
  G4double vac_density = 1.225e-7*mg/cm3; 
  G4double pressure    = 1.e-7*bar;
  G4double temperature = STP_Temperature + 20*kelvin;
  G4Material* Vacuum = new G4Material("Vacuum_7", vac_density, 1,kStateGas, temperature, pressure);
  Vacuum->AddMaterial(Air, 1.); 

#ifdef G4VIS_USE
  // usefull colors
  // G4VisAttributes* HallVisAtt = new G4VisAttributes(false,G4Colour(0.,1.,0.));
  // G4VisAttributes* plateVisAtt = new G4VisAttributes(true,G4Colour(0.89,0.45,1.13));
  // G4VisAttributes* detectorVisAtt = new G4VisAttributes(true,G4Colour(0.89,0.45,0.13));
  // G4VisAttributes* detectorVisAtt2 = new G4VisAttributes(true,G4Colour(0.19,0.45,0.83));
  // G4VisAttributes* detectorVisAtt3 = new G4VisAttributes(true,G4Colour(0.19,1.45,0.83));

  //nice colors
  G4VisAttributes* HallVisAtt = new G4VisAttributes(false,G4Colour(0.,1.,0.));
  G4VisAttributes* plateVisAtt = new G4VisAttributes(true,G4Colour(0.2, 0.2, 0.2));
  G4VisAttributes* chamberVisAtt = new G4VisAttributes(true,G4Colour(0.1, 0.1, 0.1));
  G4VisAttributes* detectorVisAtt = new G4VisAttributes(true,G4Colour(0.89,0.45,0.13));
  G4VisAttributes* detectorVisAtt2 = new G4VisAttributes(true,G4Colour(0.89,0.45,0.13));
  G4VisAttributes* detectorVisAtt3 = new G4VisAttributes(true,G4Colour(0.89,0.45,0.13));
  // plateVisAtt->SetForceWireframe(true);
  // chamberVisAtt->SetForceWireframe(true);
  // detectorVisAtt->SetForceWireframe(true);
  // detectorVisAtt2->SetForceWireframe(true);
  // detectorVisAtt3->SetForceWireframe(true);
  
#endif

  //------------------------------ experimental hall (world volume)
  G4double expHall_x = 3.0*m;
  G4double expHall_y = 3.0*m;
  G4double expHall_z = 3.0*m;
  G4Box* experimentalHall_box = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  G4LogicalVolume *experimentalHall_log = new G4LogicalVolume(experimentalHall_box,Vacuum,"expHall_log",0,0,0);
#ifdef G4VIS_USE
  experimentalHall_log->SetVisAttributes(HallVisAtt);
#endif
  G4VPhysicalVolume *experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0);

  if(fset->ChamberON()>0){
    //------------------------------ vaccumchamber
    //G4Boxes are defined from the center, i.e. length = length/2 on both sides
    G4Box* chamber_vol = new G4Box("chamber_vol",fset->ChamberHeight()*mm/2,fset->ChamberWidth()*mm/2,fset->ChamberLength()*mm/2);
    G4Box* chamber_hole = new G4Box("chamber_hole",fset->ChamberHeight()*mm/2-fset->ChamberThickness()*mm,fset->ChamberWidth()*mm/2-fset->ChamberThickness()*mm,fset->ChamberLength()*mm/2-fset->ChamberThickness()*mm);
    
    G4SubtractionSolid* chamber_box = new G4SubtractionSolid("chamber_box",chamber_vol,chamber_hole);
    G4LogicalVolume *chamber_log = new G4LogicalVolume(chamber_box, Steel,"chamber_log");
#ifdef G4VIS_USE
    chamber_log->SetVisAttributes(chamberVisAtt);
#endif
    G4VPhysicalVolume *chamber_phys = new G4PVPlacement(NULL,G4ThreeVector(0,0,fset->ChamberPositionZ()*mm),chamber_log,"chamber",experimentalHall_log,false,0);    
  }

  //------------------------------ beam stopper plate (Al)  
  G4Box* plate_box = new G4Box("plate_box",fset->PlateHeight()*mm/2,fset->PlateWidth()*mm/2,fset->PlateThickness()*mm/2);
  G4LogicalVolume *plate_log = new G4LogicalVolume(plate_box, Al,"plate_log");
#ifdef G4VIS_USE
  plate_log->SetVisAttributes(plateVisAtt);
#endif
  //place plate at the origin
  G4VPhysicalVolume *plate_phys = new G4PVPlacement(NULL,G4ThreeVector(-(fset->PlatePositionX()*mm-fset->PlateHeight()/2*mm),fset->PlatePositionY()*mm-fset->PlateWidth()/2*mm,0),plate_log,"plate",experimentalHall_log,false,0);



  //------------------------------ detector array
  //needs sensitive detector manager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  //32 G4Boxes for the crystals, and 32 sensitive detectors
  G4Box* detector_box[32];
  G4LogicalVolume* detector_log[32];
  G4VPhysicalVolume* detector_phys[32];
  SensitiveDetector* sensitive_detector[32];
  char name[100];

  //loops over x and y coordinates, array is 4x8
  for(int ix=0;ix<8;ix++){
    for(int iy=0;iy<4;iy++){
      //crystal number counts from 0 - 31

      //    0    1    2    3   |  iy/ix
      //---------------------- |-------
      //    c0   c1   c2   c3  |  0
      //    c4   c5   c6   c7  |  1
      //    c8   c9  c10  c11  |  2
      //   c12  c13  c14  c15  |  3
      //   c16  c17  c18  c19  |  4
      //   c20  c21  c22  c23  |  5
      //   c24  c25  c26  c27  |  6
      //   c28  c29  c30  c31  |  7

      int cry = ix*4+iy;
      sprintf(name, "detector_box_%d", cry); 
      detector_box[cry]= new G4Box(name,fset->CrystalHeight()*mm/2,fset->CrystalWidth()*mm/2,fset->CrystalDepth()*mm/2);
      sprintf(name, "detector_log_%d", cry); 
      detector_log[cry] = new G4LogicalVolume(detector_box[cry], CsI,name);
      
#ifdef G4VIS_USE
      detector_log[cry]->SetVisAttributes(detectorVisAtt);
      if(ix%2==0 && iy%2==0)
	detector_log[cry]->SetVisAttributes(detectorVisAtt2);
      if(ix==0 && iy==0)
	detector_log[cry]->SetVisAttributes(detectorVisAtt3);
#endif
      
      // -----placement of the detectors:------
      // coordinate system has center in the center of the plate.
      // 8 crystals in x (up-down) direction 
      // 4 crystals in y direction
      // position of the first crystal is:
      //   (-4 + 0.5) * (detectorsize + gap)
      // shift by half a crystal because
      // positions are with respect to the center of the box!
      double x = (ix-4+0.5) * fset->CrystalHeight()*mm;
      double y = (iy-2+0.5) * fset->CrystalWidth()*mm;
      sprintf(name, "detector_phys_%d", cry); 
      detector_phys[cry] = new G4PVPlacement(NULL,G4ThreeVector(x,y,-(fset->PlateDistance()*mm+fset->CrystalDepth()/2*mm)),detector_log[cry],name,experimentalHall_log,false,0);
      
      // -----creating sensisitve detectors and adding them to the sensitive detector manager
      sprintf(name, "crystal_%d", cry); 
      sensitive_detector[cry] = new SensitiveDetector(name);
      SDman->AddNewDetector(sensitive_detector[cry]);
      detector_log[cry]->SetSensitiveDetector(sensitive_detector[cry]);
    }// loop over iy = 0..4
  }// loop over ix = 0..8

  return experimentalHall_phys;
}

