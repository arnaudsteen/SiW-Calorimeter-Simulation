import os
muonEnergy=10 # GeV
nevent=10
seed=1

args = str(muonEnergy)+" "+str(nevent)+" "+str(seed)
print args
os.system("source script/runMuonPlusFSR.sh "+args)
