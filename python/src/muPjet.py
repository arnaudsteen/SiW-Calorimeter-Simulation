import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(minEnergy=0.0,maxEnergy=20.0,maxPhi=2*math.pi,minEta=1.5,sigmaTheta=6*math.pi/180,centralTheta=2*math.pi/180)
evtGen.generateMuonPlusJet(nParticles=10,muonEnergy=20)
