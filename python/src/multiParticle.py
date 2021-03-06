import eventGeneration
import math
import random
evtGen=eventGeneration.eventGeneration(maxPhi=2*math.pi,minEta=1.5,sigmaTheta=8*math.pi/180)
particles=["gamma","pi-","pi+","proton","pi0","e+","e-","kaon0L","kaon+","kaon-"]
n=10
particleName=[]
particleEnergy=[]
for i in range(0,n):
    particleName.append( particles[ random.randint(0,len(particles)-1) ] )
    energy=random.uniform(0,100)
    particleEnergy.append(energy)
evtGen.generateMultiParticle(particleName,particleEnergy)
