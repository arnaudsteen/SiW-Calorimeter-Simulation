import eventGeneration
import math

evtGen=eventGeneration.eventGeneration(minEnergy=0.0,maxEnergy=2.0,maxPhi=2*math.pi,maxTheta=10*math.pi/2/180)
evtGen.generateFSRMuon(muonEnergy=50)
evtGen.generateMuonPlusJet(nParticles=10,muonEnergy=10)
evtGen.generateSingleParticle("pi+",40)

particleName=["e+","e-"]
particleEnergy=[5.0,6.0]
evtGen.generateMultiParticle(particleName,particleEnergy)


