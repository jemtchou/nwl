# NWL

## Description of the project
Nuclear Well Logging Simulation

## Installation
1. Install Geant4 (http://cern.ch/geant4)
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

Description of source: Type  OR   Spectrum - Direction - Particle

Description of materials

Description of geometry

Description of sensitive detector

Description of output

## Contributing
* It's GPL
* Raise issue and discuss change first
* Fork it
* Make new branch for change
* Make changes
* Send pull request

# Credits
 Include a section for credits in order to highlight and link to the authors of your project.

# License
NWL is released as open source software under the GPL v3 license, see the LICENSE file in the project root for the full license text.

NWL has been developed by JINR in partnership with the Moscow State University.
