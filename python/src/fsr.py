import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(minEnergy=0.0,maxEnergy=2.0,maxPhi=2*math.pi,minEta=1.5,sigmaTheta=8*math.pi/180)
evtGen.generateFSRMuon(muonEnergy=50)
