#include "LCIOWriter.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"

LCIOWriter::LCIOWriter(std::string fileName) :
  eventNumber(0),
  runNumber(0),
  detectorName(std::string("siwecal")),
  lcWriter(NULL),
  evt(NULL),
  runHeader(NULL),
  step(NULL),
  particle(NULL)
{
  lcWriter = LCFactory::getInstance()->createLCWriter();
  if( fileName.find(".slcio")<fileName.size() )
    lcWriter->open(fileName,LCIO::WRITE_NEW) ;
  else
    lcWriter->open(fileName+std::string(".slcio"),LCIO::WRITE_NEW) ;
}

LCIOWriter::~LCIOWriter()
{
  std::cout << "delete LCIOWriter" << std::endl;
  for(std::map<std::string,IMPL::LCCollectionVec*>::iterator it= collectionMap.begin(); it!=collectionMap.end(); ++it)
    delete it->second;
  delete lcWriter;
  delete evt; 
  delete runHeader;
  delete step;
  delete particle;
}

void LCIOWriter::createLCEvent(const std::string &name)
{
  eventNumber++;
  evt =  new IMPL::LCEventImpl();
  evt->setRunNumber(runNumber);
  evt->setEventNumber(eventNumber);
  evt->setTimeStamp(0);
  evt->setDetectorName(name);
  hitMap.clear();
  collectionMap.clear();
}

void LCIOWriter::createOutputCollection(std::string colType, std::string colName)
{
  IMPL::LCCollectionVec *col;
  if( colType==std::string("simcalorimeterhit") ){
    std::cout << "create simcalorimeterhit collection : " << colName << std::endl;
    col=new IMPL::LCCollectionVec( LCIO::SIMCALORIMETERHIT );
    IMPL::LCFlagImpl chFlag(0) ;
    EVENT::LCIO bitinfo;
    chFlag.setBit(bitinfo.CHBIT_LONG ) ;   // sim calorimeter hit position
    chFlag.setBit(bitinfo.CHBIT_ID1 ) ;    // cell ID 
    chFlag.setBit(bitinfo.CHBIT_STEP ) ;   // step info
    col->setFlag( chFlag.getFlag()  ) ;   
  }
  else if( colType==std::string("genericobject") ){
    std::cout << "create genericobject collection : " << colName << std::endl;
     col=new IMPL::LCCollectionVec(LCIO::LCGENERICOBJECT) ;
  }

  std::pair< std::string , IMPL::LCCollectionVec* > p(colName,col);
  if( collectionMap.find(colName)!=collectionMap.end() ){
    std::cout << "----> ERROR collection " << colName << " is already existing. abort" << std::endl;
    std::abort();
  }
  else{
    collectionMap.insert(p);
  }
}

void LCIOWriter::fillSimCaloHitCollection(std::string outColName, std::string inColName, std::vector<SiWTrackHit*> &siWTrackHitVec, IMPL::MCParticleImpl *motherPart)
{
  step = new MCParticleCont();
  
  UTIL::CellIDEncoder<SimCalorimeterHitImpl> IDcoder("M:3,S-1:3,I:9,J:9,K-1:6",collectionMap[outColName]);
  double lengthUnit=CLHEP::mm;
  double energyUnit=CLHEP::GeV;
  double timeUnit=CLHEP::ns;

  int kShift=0;
  if( inColName.find("0")<inColName.size() ){
    kShift=0;
  }
  else if( inColName.find("1")<inColName.size() ){
    kShift=1;
  }
  else{
    G4cout << "Problem in input collection name : " << inColName << ", which does not contain 0 or 1 -> std::abort() in LCIOWriter::fillSimCaloHitCollection(std::string outColName, std::string inColName, std::vector<SiWTrackHit*> &siWTrackHitVec, IMPL::MCParticleImpl *motherPart)" << G4endl;
    std::abort();
  }

  for (std::vector<SiWTrackHit*>::iterator it=siWTrackHitVec.begin(); it != siWTrackHitVec.end(); it++) {
    step->Particle = motherPart;
    step->Energy = (*it)->energyDeposited()/energyUnit;
    step->Time = (*it)->time()/timeUnit;;
    step->PDG = (*it)->pdgID();
    step->StepPosition[0] = (*it)->entrancePoint().x()/lengthUnit + (*it)->deltaPosition().x()/2/lengthUnit;
    step->StepPosition[1] = (*it)->entrancePoint().y()/lengthUnit + (*it)->deltaPosition().y()/2/lengthUnit;
    step->StepPosition[2] = (*it)->entrancePoint().z()/lengthUnit + (*it)->deltaPosition().z()/2/lengthUnit;

    int I = (int)(DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2+step->StepPosition[0])/DetectorConstruction::Instance()->getEcalGeomParams().pixelSize ;
    int J = (int)(DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2+step->StepPosition[1])/DetectorConstruction::Instance()->getEcalGeomParams().pixelSize ;
    int K = 2*(*it)->replicaCopyNumber() + kShift;
    
    int key=IJKtoKey(I,J,K);
    if (hitMap.count(key)==0){
      hitMap[key]=new IMPL::SimCalorimeterHitImpl();
      IDcoder["M"]=0;
      IDcoder["S-1"]=3; //stave 4
      IDcoder["I"]=I; //first is at 0
      IDcoder["J"]=J; //first is at 0
      IDcoder["K-1"]=K; //first layer should be K-1=0
      IDcoder.setCellID( hitMap[key] );
      float pos[3]; //la position est le centre de la cellule dans les 3 directions
      pos[0] = (I+0.5)*DetectorConstruction::Instance()->getEcalGeomParams().pixelSize - DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2;
      pos[1] = (J+0.5)*DetectorConstruction::Instance()->getEcalGeomParams().pixelSize - DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2;

      pos[2] = (kShift==0) ?
	-DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 +
	DetectorConstruction::Instance()->getEcalGeomParams().Silicon0RelativePositionZ +
	DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness * ( K/2+0.5 ) :

	-DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 +
	DetectorConstruction::Instance()->getEcalGeomParams().Silicon1RelativePositionZ +
	DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness * ( K/2+0.5 ) ;
      //K/2 = replicaCopyNumber()
      hitMap[key]->setPosition(pos);
    }
    hitMap[key]->addMCParticleContribution( step->Particle,
					    step->Energy,
					    step->Time,
					    step->PDG, 
					    step->StepPosition
					    ) ;
  }
}

void LCIOWriter::GenerateUniformNoise(IMPL::MCParticleImpl *motherPart, std::string outColName)
{
  float noiseRate=0.;
  int cellCount=0;
  UTIL::CellIDEncoder<SimCalorimeterHitImpl> IDcoder("M:3,S-1:3,I:9,J:9,K-1:6",collectionMap[outColName]);
  for(int k=0; k<DetectorConstruction::Instance()->getEcalGeomParams().nModulePerSection*2; k++)
    for(int j=0; j<DetectorConstruction::Instance()->getEcalGeomParams().nPixels; j++)
      for(int i=0; i<DetectorConstruction::Instance()->getEcalGeomParams().nPixels; i++){
	cellCount++;
	if( CLHEP::HepRandom::getTheEngine()->flat()>DetectorConstruction::Instance()->getEcalGeomParams().noiseRate )
	  continue;
	noiseRate++;
	int key=IJKtoKey(i,j,k);
	if(hitMap.count(key)==0){
	  hitMap[key]=new IMPL::SimCalorimeterHitImpl();
	  IDcoder["M"]=0;
	  IDcoder["S-1"]=3;
	  IDcoder["I"]=i;
	  IDcoder["J"]=j;
	  IDcoder["K-1"]=k;
	  IDcoder.setCellID( hitMap[key] );
	  float pos[3]; //la position est le centre de la cellule dans les 3 directions
	  pos[0] = (i+0.5)*DetectorConstruction::Instance()->getEcalGeomParams().pixelSize - DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2;
	  pos[1] = (j+0.5)*DetectorConstruction::Instance()->getEcalGeomParams().pixelSize - DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY/2;
	  
	  pos[2] = (k%2==0) ?
	    -DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 +
	    DetectorConstruction::Instance()->getEcalGeomParams().Silicon0RelativePositionZ +
	    DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness * ( k+0.5 ) :
	    
	    -DetectorConstruction::Instance()->getEcalGeomParams().calorThickness/2 +
	    DetectorConstruction::Instance()->getEcalGeomParams().Silicon1RelativePositionZ +
	    DetectorConstruction::Instance()->getEcalGeomParams().moduleThickness * ( k+0.5 ) ;
	  hitMap[key]->setPosition(pos);
	}
	float pos[3];
	for(int l=0; l<3; l++)
	  pos[l]=hitMap[key]->getPosition()[l];
	hitMap[key]->addMCParticleContribution( motherPart,
						6e-5,
						100,
						0,
						pos
					       );
      }
}

void LCIOWriter::writeEvent(IMPL::MCParticleImpl* motherPart)
{
  for (std::map<int,IMPL::SimCalorimeterHitImpl* >::iterator itmap=hitMap.begin(); itmap != hitMap.end(); itmap++)
    collectionMap[std::string("SiWEcalSimHit")]->addElement(itmap->second);
  
  for(std::map<std::string , IMPL::LCCollectionVec*>::iterator it=collectionMap.begin(); it!=collectionMap.end(); ++it){
    evt->addCollection(it->second,it->first);
  }

  FloatVec momentum;
  momentum.push_back(motherPart->getMomentum()[0]);
  momentum.push_back(motherPart->getMomentum()[1]);
  momentum.push_back(motherPart->getMomentum()[2]);
  evt->parameters().setValue("CalorimeterSize", (float)DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY) ;
  evt->parameters().setValue("CalorimeterThickness", (float)DetectorConstruction::Instance()->getEcalGeomParams().calorThickness) ;
  evt->parameters().setValues("ParticleMomentum", momentum) ;
  FloatVec position;
  position.push_back(motherPart->getVertex()[0]);
  position.push_back(motherPart->getVertex()[1]);
  position.push_back(motherPart->getVertex()[2]);
  evt->parameters().setValues("GunPosition", position) ;

  FloatVec magField;
  magField.push_back(DetectorConstruction::Instance()->getEcalGeomParams().magField.x());
  magField.push_back(DetectorConstruction::Instance()->getEcalGeomParams().magField.y());
  magField.push_back(DetectorConstruction::Instance()->getEcalGeomParams().magField.z());
  evt->parameters().setValues("Magneticfield", magField) ;
  
  evt->parameters().setValue("noiseRate",(float)DetectorConstruction::Instance()->getEcalGeomParams().noiseRate);
  
  LCTOOLS::dumpEvent(evt);
  lcWriter->writeEvent(evt);

}



void LCIOWriter::writeGenericEvent(std::vector<IMPL::MCParticleImpl*> particles)
{
  for (std::map<int,IMPL::SimCalorimeterHitImpl* >::iterator itmap=hitMap.begin(); itmap != hitMap.end(); itmap++)
    collectionMap[std::string("SiWEcalSimHit")]->addElement(itmap->second);
  
  for(std::map<std::string , IMPL::LCCollectionVec*>::iterator it=collectionMap.begin(); it!=collectionMap.end(); ++it){
    evt->addCollection(it->second,it->first);
  }

  evt->parameters().setValue("NumberOfParticles", (int)particles.size() ) ;
  evt->parameters().setValue("CalorimeterSize", (float)DetectorConstruction::Instance()->getEcalGeomParams().calorSizeXY) ;
  evt->parameters().setValue("CalorimeterThickness", (float)DetectorConstruction::Instance()->getEcalGeomParams().calorThickness) ;
  FloatVec position;
  position.push_back(particles.at(0)->getVertex()[0]);
  position.push_back(particles.at(0)->getVertex()[1]);
  position.push_back(particles.at(0)->getVertex()[2]);
  evt->parameters().setValues("GunPosition", position) ;

  std::ostringstream os( std::ostringstream::ate );

  for(int pid=0; pid<particles.size(); pid++){
    FloatVec momentum;
    momentum.push_back(particles[pid]->getMomentum()[0] );
    momentum.push_back(particles[pid]->getMomentum()[1]);
    momentum.push_back(particles[pid]->getMomentum()[2]);
    
    os.str("");
    os << std::string("particleMomentum_") << pid ;
    evt->parameters().setValues(os.str(), momentum) ;
    os.str("");
    os << std::string("particlePDG_") << pid ;
    evt->parameters().setValue(os.str(),particles[pid]->getPDG());
  }
  
  FloatVec magField;
  magField.push_back(DetectorConstruction::Instance()->getEcalGeomParams().magField.x());
  magField.push_back(DetectorConstruction::Instance()->getEcalGeomParams().magField.y());
  magField.push_back(DetectorConstruction::Instance()->getEcalGeomParams().magField.z());
  evt->parameters().setValues("Magneticfield", magField) ;
  
  evt->parameters().setValue("noiseRate",(float)DetectorConstruction::Instance()->getEcalGeomParams().noiseRate);
  
  LCTOOLS::dumpEvent(evt);
  lcWriter->writeEvent(evt);

}
