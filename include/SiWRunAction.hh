#ifndef SiWRunAction_h
#define SiWRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "g4root.hh"

class G4Run;

class SiWRunAction : public G4UserRunAction
{
public:
  SiWRunAction();
  ~SiWRunAction();
  
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);
  G4Run* GenerateRun();
  
};

class SiWGenericRunAction : public G4UserRunAction
{
public:
  SiWGenericRunAction();
  ~SiWGenericRunAction();
  
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);
  G4Run* GenerateRun();
  
};

#endif
