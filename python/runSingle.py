import os
particle="mu-"
energy=10 # GeV
nevent=100

Nlayer=14 # 2 silicon layers per layer
SiThickness=0.3 # mm

seed=0
model="QGSP_BERT"

args = " "+particle+" "+str(energy)+" "+str(nevent)+" "+str(Nlayer)+" "+str(SiThickness)+" "+str(seed)+" "+model
print args
os.system("source script/siwecal-simu.sh "+args)
