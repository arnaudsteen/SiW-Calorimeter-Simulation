import os
muonEnergy=20 # GeV
nParticles=10
nevent=100
seed=0

args = str(muonEnergy)+" "+str(nParticles)+" "+str(nevent)+" "+str(seed)
print args
os.system("source script/runMuonPlusJet.sh "+args)
