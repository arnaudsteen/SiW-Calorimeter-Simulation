#!/bin/bash

#nparticle=9
#particle="mu- pi- pi+ proton e+ e- gamma gamma kaon0L"
#energy="10 10 10 10 10 10 10 10 10"
nparticle=1
particle="gamma"
gamma_energy=$1
energies="${gamma_energy}"
nevent=2000

Nlayer=20
SiThickness=0.3

seed=$2
model=QGSP_BERT

magField="0 0 0"
noiseRate=0.0
GaussianMean=0.2
GaussianSigma=0.0

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
echo "Energies = " ${energies} "GeV"
echo "Particles: " ${particle}
echo "Nevent = " ${nevent}
echo "Nlayer = " ${Nlayer}
echo "Silicon Thickness = " ${SiThickness}

cat > autorun.mac <<EOF
/run/initialize

/MultipleGunParameter/gaussianMean $GaussianMean
#/MultipleGunParameter/gaussianSigma $GaussianSigma

/MultipleGunParameter/NParticles $nparticle
/MultipleGunParameter/ParticlesName $particle
/MultipleGunParameter/ParticlesEnergy $energies
/run/beamOn $nevent

EOF

cat > autogeom.mac <<EOF
/DetectorConstruction/Nlayer $Nlayer
/DetectorConstruction/SiThickness $SiThickness
/DetectorConstruction/NoiseRate $noiseRate
/DetectorConstruction/MagField $magField
/DetectorConstruction/NumberOfPixels 128
EOF

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:.

./SiWMultipleGunSimu -m autorun.mac -g autogeom.mac -s ${seed}
rm autorun.mac
rm autogeom.mac

mv toto.slcio single_gamma${gamma_energy}GeV.slcio
