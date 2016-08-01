#!/bin/bash

particle=$1
energy=$2
nevent=$3

Nlayer=$4
SiThickness=$5

seed=$6
model=$7

noiseRate=0.
GunOptionPosition=beamtest #default, random, cosmic
RandomMaxPosition=250
GunOptionMomentum=gaus #gaus, solidAngle, uniform, normal, cosmic_gaus, cosmic_uniform
GaussianMean=0.
GaussianSigma=0.1
UniformParameter=0.3

XPositionSigma=5 # mm
YPositionSigma=5 # mm

#mu std conf : 
#cosmic cosmic_gaus 0.0 1.0
#random 0.1 gaus 0.0 0.1
#default uniform 0.3

ILCSOFT="/Users/arnaudsteen/ilcsoft/v01-17-09"
source ${ILCSOFT}/init_ilcsoft.sh
export ROOTSYS=${ILCSOFT}/root/v5.34..34/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib
export PATH=$PATH:$ROOTSYS/bin
source ${ILCSOFT}/geant4/10.01/share/Geant4-10.1.0/geant4make/geant4make.sh
source ${ILCSOFT}/lcio/v02-07/build_env.sh
source ${ILCSOFT}/lcio/v02-07/setup.sh
export QTHOME=$QTDIR

if
    [[ $model == "" ]]
    then
    model=FTFP_BERT
fi
export PHYSLIST=${model}

echo "Physics list = "$model
echo "Energy = " ${energy} "GeV"
echo "Particle: " ${particle}
echo "Nevent = " ${nevent}
echo "Nlayer = " ${Nlayer}
echo "Silicon Thickness = " ${SiThickness}
cat > autorun.mac <<EOF
/run/initialize

/GunParameter/gunOptionPosition $GunOptionPosition
/GunParameter/randomMaxPosition $RandomMaxPosition

/GunParameter/gunOptionMomentum $GunOptionMomentum
/GunParameter/gaussianMean $GaussianMean
/GunParameter/gaussianSigma $GaussianSigma
/GunParameter/uniformParameter $UniformParameter
/GunParameter/xPositionSigma $XPositionSigma
/GunParameter/yPositionSigma $YPositionSigma

/gun/particle $particle
/gun/energy $energy GeV
/run/beamOn $nevent

EOF

cat > autogeom.mac <<EOF
/DetectorConstruction/Nlayer $Nlayer
/DetectorConstruction/SiThickness $SiThickness
/DetectorConstruction/NoiseRate $noiseRate

EOF

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.

./bin/SiWSimu -m autorun.mac -g autogeom.mac -s ${seed}
rm autorun.mac
rm autogeom.mac

mv toto.slcio single_${particle}_${energy}GeV_${seed}.slcio
mv testOutPut.root single_${particle}_${energy}GeV_${seed}.root
