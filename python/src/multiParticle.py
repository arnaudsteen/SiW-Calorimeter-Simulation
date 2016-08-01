import eventGeneration
import math
evtGen=eventGeneration.eventGeneration(maxPhi=2*math.pi,maxTheta=10*math.pi/2/180)
particleName=["e+","e-"]
particleEnergy=[5.0,6.0]
evtGen.generateMultiParticle(particleName,particleEnergy)