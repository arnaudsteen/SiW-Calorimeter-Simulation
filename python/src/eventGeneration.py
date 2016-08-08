import math
import random
import numpy as np

class eventGeneration:

    minEnergy=0
    maxEnergy=0
    maxPhi=0
    maxTheta=0

    def __init__(self,minEnergy=0.0,maxEnergy=1.0,maxPhi=2*math.pi,minEta=1.5,centralTheta=0.0,sigmaTheta=10.0*math.pi/180.0):
        self.minEnergy=minEnergy
        self.maxEnergy=maxEnergy
        self.maxPhi=maxPhi
        self.minEta=minEta
        self.maxTheta=2*math.atan( math.exp(-self.minEta) )
        self.centralTheta=centralTheta
        self.sigmaTheta=sigmaTheta
        #self.theta=self.maxTheta + np.spacing(1.0)
        
    def generateSingleParticle(self,particleName,particleEnergy):
        config=open("config.txt","w")
        self.theta=self.maxTheta + np.spacing(1.0)
        while(  self.theta>self.maxTheta or self.theta<0 ):
            self.theta=np.random.normal(self.centralTheta,self.sigmaTheta)
            
        eta=-math.log( math.tan(self.theta/2) )
        phi=random.uniform(0,self.maxPhi)
        config.write( particleName + " " + str(particleEnergy) + " " + str(eta) + " " + str(phi) )
        config.close()
        
    def generateMultiParticle(self,particleName,particleEnergy):
        config=open("config.txt","w")
        if( len(particleName)!=len(particleEnergy) ):
            print "ERROR : particleName and particleEnergy should have the same size -> return"
            return
        for i in range(0,len(particleName)):
            self.theta=self.maxTheta + np.spacing(1.0)
            while(  self.theta>self.maxTheta or self.theta<0 ):
                self.theta=np.random.normal(self.centralTheta,self.sigmaTheta)
                
            eta=-math.log( math.tan(self.theta/2) )
            phi=random.uniform(0,self.maxPhi)
            config.write( particleName[i] + " " + str(particleEnergy[i]) + " " + str(eta) + " " + str(phi) )
            if( i<len(particleName)-1 ):
                config.write("\n")
        config.close()

    def generateFSRMuon(self,muonEnergy=50):
        config=open("config.txt","w")
        self.theta=self.maxTheta + np.spacing(1.0)
        while(  self.theta>self.maxTheta or self.theta<0 ):
            self.theta=np.random.normal(self.centralTheta,self.sigmaTheta)
            
        eta=-math.log( math.tan(self.theta/2) )
        phi=random.uniform(0,self.maxPhi)
        gammaEnergy=random.uniform(self.minEnergy,self.maxEnergy)
        config.write( "mu-" + " " + str(muonEnergy) + " " + str(eta) + " " + str(phi) )
        config.write("\n")
        config.write( "gamma" + " " + str(gammaEnergy) + " " + str(eta) + " " + str(phi) )
        config.close()

    def generateMuonPlusJet(self,nParticles=1,muonEnergy=10):
        particles=["gamma","pi-","pi+","proton","pi0","e+","e-","kaon0L","kaon+","kaon-"]
        config=open("config.txt","w")
        self.theta=self.maxTheta + np.spacing(1.0)
        while(  self.theta>self.maxTheta or self.theta<0 ):
            self.theta=np.random.normal(self.centralTheta,self.sigmaTheta)
            
        eta=-math.log( math.tan(self.theta/2) )
        phi=random.uniform(0,self.maxPhi)
        config.write( "mu-" + " " + str(muonEnergy) + " " + str(eta) + " " + str(phi) )
        for i in range(0,nParticles):
            config.write("\n")
            part=particles[ random.randint(0,len(particles)-1) ]
            self.theta=self.maxTheta + np.spacing(1.0)
            while(  self.theta>self.maxTheta or self.theta<0 ):
                self.theta=np.random.normal(self.centralTheta,self.sigmaTheta)
            
            eta=-math.log( math.tan(self.theta/2) )
            phi=random.uniform(0,self.maxPhi)
            energy=random.uniform(self.minEnergy,self.maxEnergy)
            if( eta<1.5 ):
                print part + " " + str(energy) + " " + str(eta) + " " + str(phi) 
            phi=random.uniform(0,self.maxPhi)
            config.write( part + " " + str(energy) + " " + str(eta) + " " + str(phi) )
        config.close()
