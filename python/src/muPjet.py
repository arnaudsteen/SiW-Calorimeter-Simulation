import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(minEnergy=0.0,maxEnergy=20.0,maxPhi=2*math.pi,maxTheta=10*math.pi/2/180)
evtGen.generateMuonPlusJet(nParticles=10,muonEnergy=20)
