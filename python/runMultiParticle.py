import os

nevent=10
seed=1

args = str(nevent)+" "+str(seed)
print args
os.system("source script/runMultiParticleSimulation.sh "+args)
