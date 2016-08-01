import os
muonEnergy=10 # GeV
nParticles=6
nevent=100
seed=16

args = str(muonEnergy)+" "+str(nParticles)+" "+str(nevent)+" "+str(seed)
print args
os.system("source script/runMuonPlusJet.sh "+args)
