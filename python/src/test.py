import eventGeneration
import math
import matplotlib.pyplot as plt
import numpy as np
import random

evtGen=eventGeneration.eventGeneration(maxPhi=2*math.pi,minEta=1.5,sigmaTheta=8*math.pi/180)
particles=["gamma","pi-","pi+","proton","pi0","e+","e-","kaon0L","kaon+","kaon-"]
n=10000
particleName=[]
particleEnergy=[]
for i in range(0,n):
    particleName.append( particles[ random.randint(0,len(particles)-1) ] )
    energy=random.uniform(0,100)
    particleEnergy.append(energy)

evtGen.generateMultiParticle(particleName,particleEnergy)
x=np.loadtxt("config.txt",dtype=str)
y=[]
for i in x:
    y.append( float(i[2]) )

num_bins = 100
multiPlot = plt.hist(y, num_bins, normed=1, facecolor='green', alpha=0.5)
plt.show()

