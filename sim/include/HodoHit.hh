/*************************************************************************
 *
 *    hit in the hodoscope
 * 
 *************************************************************************/

#ifndef HodoHit_h
#define HodoHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"
#include "G4Types.hh"
#include "G4String.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

class HodoHit;

typedef G4THitsCollection<HodoHit> HitsCollection;

extern G4Allocator<HodoHit> HitAllocator;


class HodoHit : public G4VHit
{
public:

  HodoHit();
  ~HodoHit();
  HodoHit(const HodoHit&);
  const HodoHit& operator=(const HodoHit&);
  int operator==(const HodoHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:
  
  void SetTrackID(G4int track){ trackID = track; };
  void SetParentID(G4int par){ parentID = par; };
  void SetEdep(G4double de){ edep = de; };
  void SetPos(G4ThreeVector xyz){ pos = xyz; };
  void SetPosInDet(G4ThreeVector xyz){ posindet = xyz; };
  void SetTrack(G4Track* Track){ track.CopyTrackInfo(*Track); };
  void SetProcName(G4String pname){ procName = pname; };
  void SetStep(G4int Step){ step = Step; };
  void SetVolume(G4String Vol){ vol = Vol; };
  void SetHodoID(G4int seg_id) {HodoID = seg_id;};
      
  G4int GetTrackID()          { return trackID; };
  G4int GetParentID()         { return parentID; };
  G4double GetEdep()          { return edep; };      
  G4ThreeVector GetPos()      { return pos; };
  G4ThreeVector GetPosInDet() { return posindet; };
  G4Track* GetTrack()         { return &track; };
  G4String GetProcName()      { return procName; };
  G4int GetStep()             { return  step; };
  G4String GetVolume()        { return vol; };
  G4int GetHodoID()      { return HodoID; };


private:
  
  G4int         trackID;
  G4int         parentID;
  G4double      edep;
  G4ThreeVector pos;
  G4ThreeVector posindet;
  G4Track       track;
  G4String      procName;
  G4int         step;
  G4String      vol;
  G4int         HodoID;
};

inline void* HodoHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HitAllocator.MallocSingle();
  return aHit;
}

inline void HodoHit::operator delete(void *aHit)
{
  HitAllocator.FreeSingle((HodoHit*) aHit);
}

#endif


