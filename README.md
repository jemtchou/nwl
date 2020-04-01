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
Configuration file consists of the following sections:

NumberOfEvents <number_of_events> # *e.g.NumberOfEvents 1000000*

LoggingType <type>  # *NeutronNeutron, NeutronGamma or GammaGamma*

Description of source: macro with Geant4 GPS commands
(GPS commands are described here: http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/html/GettingStarted/generalParticleSource.html )

Description of materials

Description of geometry

Description of sensitive detector

Description of output
1D histograms
2D histograms
Ntuple (a table with particle information records)
Output is stored in CSV format

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
