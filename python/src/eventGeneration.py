import math
import random

class eventGeneration:

    minEnergy=0
    maxEnergy=0
    maxPhi=0
    maxTheta=0

    def __init__(self,minEnergy=0.0,maxEnergy=1.0,maxPhi=2*math.pi,maxTheta=math.pi/2):
        self.minEnergy=minEnergy
        self.maxEnergy=maxEnergy
        self.maxPhi=maxPhi
        self.maxTheta=maxTheta

    def generateSingleParticle(self,particleName,particleEnergy):
        config=open("config.txt","w")
        theta=random.uniform(0,self.maxTheta)
        eta=-math.log( math.tan(theta/2) )
        phi=random.uniform(0,self.maxPhi)
        config.write( particleName + " " + str(particleEnergy) + " " + str(eta) + " " + str(phi) )
        config.close()
        
    def generateMultiParticle(self,particleName,particleEnergy):
        config=open("config.txt","w")
        theta=random.uniform(0,self.maxTheta)
        eta=-math.log( math.tan(theta/2) )
        phi=random.uniform(0,self.maxPhi)
        if( len(particleName)!=len(particleEnergy) ):
            print "ERROR : particleName and particleEnergy should have the same size -> return"
            return
        for i in range(0,len(particleName)):
            theta=random.uniform(0,self.maxTheta)
            eta=-math.log( math.tan(theta/2) )
            phi=random.uniform(0,self.maxPhi)
            config.write( particleName[i] + " " + str(particleEnergy[i]) + " " + str(eta) + " " + str(phi) )
            if( i<len(particleName)-1 ):
                config.write("\n")
        config.close()

    def generateFSRMuon(self,muonEnergy=50):
        config=open("config.txt","w")
        theta=random.uniform(0,self.maxTheta)
        eta=-math.log( math.tan(theta/2) )
        phi=random.uniform(0,self.maxPhi)
        gammaEnergy=random.uniform(self.minEnergy,self.maxEnergy)
        config.write( "mu-" + " " + str(muonEnergy) + " " + str(eta) + " " + str(phi) )
        config.write("\n")
        config.write( "gamma" + " " + str(gammaEnergy) + " " + str(eta) + " " + str(phi) )
        config.close()

    def generateMuonPlusJet(self,nParticles=1,muonEnergy=10):
        particles=["gamma","pi-","pi+","proton","pi0","e+","e-","kaon0L","kaon+","kaon-"]
        config=open("config.txt","w")
        theta=random.uniform(0,self.maxTheta)
        eta=-math.log( math.tan(theta/2) )
        phi=random.uniform(0,self.maxPhi)
        config.write( "mu-" + " " + str(muonEnergy) + " " + str(eta) + " " + str(phi) )
        for i in range(0,nParticles):
            config.write("\n")
            part=particles[ random.randint(0,len(particles)-1) ]
            energy=random.uniform(self.minEnergy,self.maxEnergy)
            theta=random.uniform(0,self.maxTheta)
            eta=-math.log( math.tan(theta/2) )
            phi=random.uniform(0,self.maxPhi)
            config.write( part + " " + str(energy) + " " + str(eta) + " " + str(phi) )
        config.close()
