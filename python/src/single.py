import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(maxPhi=2*math.pi,maxTheta=10*math.pi/2/180)
particleName="e-"
particleEnergy=5.0
evtGen.generateSingleParticle(particleName,particleEnergy)
