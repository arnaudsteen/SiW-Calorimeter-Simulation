import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(minEnergy=0.0,maxEnergy=20.0,maxPhi=2*math.pi,minEta=1.5,sigmaTheta=8*math.pi/180)
particleName="e-"
particleEnergy=5.0
evtGen.generateSingleParticle(particleName,particleEnergy)
