#!/bin/bash

# adapt these paths before running scripts
PATH_TO_ILCSOFT="/Users/arnaudsteen/ilcsoft/v01-17-09"
PATH_TO_SiWSimulation="/Users/arnaudsteen/HGCAL/TestSim"
#

muonEnergy=$1
nParticles=$2
nevent=$3
seed=$4

outputFileName=${muonEnergy}GeVMuon-${nParticles}Particles-${nevent}Events-Seed${seed}.slcio

model=QGSP_BERT

Nlayer=20
SiThickness=0.3
npixels=192
noiseRate=0.

source ${PATH_TO_ILCSOFT}/init_ilcsoft.sh
export ROOTSYS=${PATH_TO_ILCSOFT}/root/v5.34..34/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib
export PATH=$PATH:$ROOTSYS/bin
source ${PATH_TO_ILCSOFT}/geant4/10.01/share/Geant4-10.1.0/geant4make/geant4make.sh
source ${PATH_TO_ILCSOFT}/lcio/v02-07/build_env.sh
source ${PATH_TO_ILCSOFT}/lcio/v02-07/setup.sh
export QTHOME=$QTDIR

if
    [[ $model == "" ]]
    then
    model=QGSP_BERT
fi
export PHYSLIST=${model}

echo "Physics list = "$model
echo "Nevent = " ${nevent}
echo "Nlayer = " ${Nlayer}
echo "Silicon Thickness = " ${SiThickness}
cat > autorun.mac <<EOF
/run/initialize

/run/beamOn $nevent

EOF

cat > autogeom.mac <<EOF
/DetectorConstruction/Nlayer $Nlayer
/DetectorConstruction/SiThickness $SiThickness
/DetectorConstruction/NoiseRate $noiseRate
/DetectorConstruction/NumberOfPixels $npixels
EOF

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.

cp ${PATH_TO_SiWSimulation}/python/src/eventGeneration.py .
cp ${PATH_TO_SiWSimulation}/python/src/muPjet.py config.py

sed -i -e "s/muonEnergy=20/muonEnergy=${muonEnergy}/g" config.py
sed -i -e "s/nParticles=10/nParticles=${nParticles}/g" config.py

echo ""
echo "******************************"
echo "---config.py---"
cat config.py
echo "******************************"
echo ""

${PATH_TO_SiWSimulation}/bin/SiWGenericSimulation -m autorun.mac -g autogeom.mac -s ${seed}
mv toto.slcio ${outputFileName}
rm autorun.mac autogeom.mac eventGeneration.py eventGeneration.pyc config.py config.txt

#### uncomment the following commands to store result on eos ####

#source /afs/cern.ch/project/eos/installation/user/etc/setup.sh
#xrdcp -f root://eosuser.cern.ch//eos/user/a/asteen/hgcal/simu/lcio/muonPlusJet/simcalorimeterhit/${outputFileName} ${outputFileName}
#rm ${outputFileName}
