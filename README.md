# NWL

## Description of the project
Nuclear Well Logging Simulation

## Installation
1. Install Geant4 (http://cern.ch/geant4) - *current version tested against Geant4.10.06*
2. Checkout NWL code
3. cd nwl
4. mkdir build
5. cmake ..
6. make

## Usage
./nwl.exe configfile.cfg        -- interactive mode
OR
./nwl.exe configfile.cfg jobId  -- batch mode, jobId is a unique job identificator (used to set the PRNG seed)

## How to write configuration file
YAML-inspired syntax.

Configuration file consists of the following sections:

**NumberOfEvents** <number_of_events> # *e.g.NumberOfEvents 1000000*

**LoggingType** <type>  # *NeutronNeutron, NeutronGamma or GammaGamma*

**GeneratorMacro**
Description of source: macro with Geant4 GPS commands
(GPS commands are described [here] (http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/GettingStarted/generalParticleSource.html) )

***Description of materials***
*Example*
**Material**
+ rock
++ Density 2.73 #g/cm3
++ Ca 40.0 #%
++ C 12.0
++ H 0.03
++ O 47.97

***Description of geometry***
*Example*
**Geometry**
+ world Vacuum Tub 0 1001 1001 0 360 () [] #(x, y, z) - placement; [x1, y1, z1, x2, y2, z2, x3, y3, z3] - rotation matrix; () [] - at point (0, 0, 0) without rotation
++ Rock rock Tub 62.0 1000.0 1000.0 0 360 () []
++ WellWater G4_WATER Tub 0.0 62.0 1000.0 0 360 () []
+++ LogTube SLW3 Tub 28.5 52.5 1000.0 0 360 () []
+++ LogAir G4_AIR Tub 0 28.5 1000 0 360 () []
++++ Source G4_Pu Tub 0.0 24.0 20.0 0 360 (0, 0, -50.0) []
++++ Shield G4_Pb Tub 0.0 28.5 30.0 0 360 (0, 0, 0.0) []
++++ DetectorBGO G4_BGO Tub 0.0 28.0 20 0 360 (0, 0, 50) [] 2.0 # volume importance

***Description of sensitive detector***
**Detector**
+ DetectorBGO

***Configuration of the output***
Output is stored in CSV format (can be changed to ROOT in nwlAnalysis.hh)

**Output**
+ H1D Energy 100 0 100     # 1D histogram
+ H1D PDG 10000 -5000 5000 # 1D histogram
+ H2D X 100 -100 100 Y 100 -100 100 # 2D histogram
+ WriteNtuple True
+ StoreAllParticles True

Histograms may contain the following particle properties:
* PDG - particle type
* Energy — kinetic energy in the particle [MeV]
* Time — time in the detector [ns since the start of the event]
* X — X in the detector [mm]
* Y — Y in the detector [mm]
* Z — Z in the detector [mm]
* ProcessID — reaction ID (list of IDs is produced in the program output at every run)
* NucleusA — A of parent nucleus
* NucleusZ — Z of parent nucleus
* DetectorID - ID of the detector (list of IDs is in the program output at every run)

# Ntuple (a table with particle information records)
Output is stored in CSV format (can be changed to ROOT in nwlAnalysis.hh)
* int EventID 	
* int TrackID 	 	
* int PDG 
* double OriginX	 
* double OriginY 
* double OriginZ 
* double Time 	
* double OriginKine 
* string OriginVolume	 
* string CreatorProcess 
* int NucleusA    
* int NucleusZ 
* string DetectorID
* double EntranceX 
* double EntranceY 
* double EntranceZ 
* double EntranceDirX 
* double EntranceDirY 
* double EntranceDirZ 
* double DetectionTime 
* double DetectionKine 
* string StopInDetectorID 
* string ReactionInDetector 
* double Weight 

## Contributing
* It's GPL
* Raise issue and discuss change first
* Fork it
* Make new branch for change
* Make changes
* Send pull request

# Credits
O.Bakina, V.Belokhin, A.Zhemchugov 

# License
NWL is released as open source software under the GPL v3 license, see the LICENSE file in the project root for the full license text.

NWL has been developed by JINR in partnership with the Moscow State University.
