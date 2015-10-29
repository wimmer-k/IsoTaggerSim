/************************************************************************
 *  
 *   standart visualisation manager
 *   (copied from a Geant4 example)
 *
 ************************************************************************/

#ifdef G4VIS_USE

#include "VisManager.hh"

using namespace std;

VisManager::VisManager () {}

void VisManager::RegisterGraphicsSystems () 
{

#ifdef G4VIS_USE_DAWNFILE
  RegisterGraphicsSystem (new G4DAWNFILE);
#endif

#ifdef G4VIS_USE_OPENGLX
  RegisterGraphicsSystem (new G4OpenGLImmediateX);
  RegisterGraphicsSystem (new G4OpenGLStoredX);
#endif

#ifdef G4VIS_USE_RAYTRACER
  RegisterGraphicsSystem (new G4RayTracer);
#endif

  if (fVerbose > 0) 
    {
      cout<<endl<<"You have successfully chosen to use the following graphics systems."<<endl;
      PrintAvailableGraphicsSystems ();
    }
}

#endif
