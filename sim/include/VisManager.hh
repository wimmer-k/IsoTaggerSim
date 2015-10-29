///////////////////////////////////////////////////////////////////////////////
/**  \file  
 *   \brief standart visualisation manager
 *   (copied from a Geant4 example)
 **/ 
///////////////////////////////////////////////////////////////////////////////

#ifndef VisManager_h
#define VisManager_h 1

#ifdef G4VIS_USE

#include "G4VisManager.hh"

// Support DAWNFILE, OpenGL and RayTracer

#ifdef G4VIS_USE_DAWNFILE
#include "G4DAWNFILE.hh"
#endif

#ifdef G4VIS_USE_OPENGLX
#include "G4OpenGLImmediateX.hh"
#include "G4OpenGLStoredX.hh"
#endif

#ifdef G4VIS_USE_RAYTRACER
#include "G4RayTracer.hh"
#endif

/// Visualisation manager
class VisManager: public G4VisManager {
public:

  VisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif

#endif
