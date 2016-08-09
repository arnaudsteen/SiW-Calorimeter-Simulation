import os
particleName="mu-"
energy=10 # GeV
nevent=1000
seed=0

args = particleName+" "+str(energy)+" "+str(nevent)+" "+str(seed)
print args
os.system("source script/runSingle.sh "+args)
