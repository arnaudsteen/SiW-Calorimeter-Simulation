import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(minEnergy=0.0,maxEnergy=2.0,maxPhi=2*math.pi,maxTheta=10*math.pi/2/180)
evtGen.generateFSRMuon(muonEnergy=50)
