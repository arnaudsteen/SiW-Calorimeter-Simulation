#ifndef LCIOWRITER_H
#define LCIOWRITER_H
#include "lcio.h"
#include <stdio.h>
#include <bitset>
#include <vector>
#include <map>
#include <utility>

#include "IMPL/LCTOOLS.h"
#include "IMPL/LCRunHeaderImpl.h" 
#include "IMPL/LCEventImpl.h" 
#include "IMPL/LCGenericObjectImpl.h"
#include <IMPL/LCCollectionVec.h>
#include "IMPL/LCEventImpl.h"
#include "IMPL/LCFlagImpl.h"

#include "IMPL/SimCalorimeterHitImpl.h"
#include "IMPL/MCParticleImpl.h"
#include "UTIL/CellIDDecoder.h"
#include <UTIL/CellIDEncoder.h>
#include <UTIL/LCSplitWriter.h>

#include <IO/LCWriter.h>

#include <cstdlib>

#include "SiWTrackHit.hh"

class LCIOWriter 
{
public:
  LCIOWriter(std::string fileName);

  ~LCIOWriter();
  
  void createLCEvent(const std::string &detName);
  
  void createOutputCollection(std::string colType, std::string colName);
  // colType could be "simcalorimeterhit" , "genericobject"

  void fillSimCaloHitCollection(std::string outColName, std::string inColName, std::vector<SiWTrackHit*> &vec, IMPL::MCParticleImpl *motherPart);
  // inColName should contain 0 or 1

  void GenerateUniformNoise(IMPL::MCParticleImpl *motherPart, std::string outColName);
  
  void writeEvent(IMPL::MCParticleImpl *motherPart);
  void writeGenericEvent(std::vector<IMPL::MCParticleImpl*> motherPart);

  inline void setRunNumber(int run){runNumber=run; eventNumber=0;}
  //setting a new run number resets event number

private:
  int eventNumber;
  int runNumber;
  std::string detectorName;
  std::map<std::string,IMPL::LCCollectionVec*> collectionMap;
  std::map<int,IMPL::SimCalorimeterHitImpl* > hitMap;
  IO::LCWriter* lcWriter;
  IMPL::LCEventImpl* evt; 
  IMPL::LCRunHeaderImpl* runHeader;
  IMPL::MCParticleCont* step;
  IMPL::LCGenericObjectImpl* particle;

  inline int IJKtoKey(int I, int J, int K){return 100*100*K+100*J+I;}
  inline int KeytoI(int key) {return key%100;}
  inline int KeytoJ(int key) {return (key/100)%100;}
  inline int KeytoK(int key) {return key/100/100;}

};
#endif
