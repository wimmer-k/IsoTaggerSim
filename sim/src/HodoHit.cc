/*************************************************************************
 *
 *    hit in the hodoscope
 * 
 *************************************************************************/

#include "HodoHit.hh"

G4Allocator<HodoHit> HitAllocator;

HodoHit::HodoHit()
  :trackID(0), edep(0), pos(0), posindet(0)
{;}

HodoHit::~HodoHit() 
{;}

HodoHit::HodoHit(const HodoHit& right)
  : G4VHit()
{
  trackID     = right.trackID;
  parentID    = right.parentID;
  edep        = right.edep;
  pos         = right.pos;
  posindet    = right.posindet;
  track.CopyTrackInfo(right.track);
  procName    = right.procName;
  step        = right.step;
  vol         = right.vol;
  HodoID     = right.HodoID;
}

const HodoHit& HodoHit::operator=(const HodoHit& right)
{
  trackID     = right.trackID;
  parentID    = right.parentID;
  edep        = right.edep;
  pos         = right.pos;
  posindet    = right.posindet;
  track.CopyTrackInfo(right.track);
  procName    = right.procName;
  step        = right.step;
  vol         = right.vol;
  HodoID     = right.HodoID;
  return *this;
}

int HodoHit::operator==(const HodoHit&) const
{
  return 0;
}

void HodoHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void HodoHit::Print()
{
  G4cout << " Hodo: " << G4endl;
  G4cout << "  trackID: " << trackID 
         << "  energy deposit: " << G4BestUnit(edep,"Energy")
	 << "  position: " << G4BestUnit(pos,"Length") << G4endl;
}




