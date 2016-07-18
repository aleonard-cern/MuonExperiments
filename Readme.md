# Muon Experiments
## Requirements:
1. Ubuntu 14.04 or more recent
2. qt5 libraries
..* you can get this simply by the command sudo apt-get install qt5-default.
..* type qmake -v in order to check that you have the qt5 version

## Installation:
1. Compile and install the CAEN USB Driver
..1. Go to CAENUSBdrvB-1.5.1
..2. make
..3. sudo make install
2. Compile and install the CAEN VME C libraries
..1. Go to CAENVMELib-2.50/lib
..2. make
..3. sudo make install
3. Compile the Muon Experiments software
..1. qmake MuonExperiments.pro
..2. make
4. Execute the programme
..1. ./MuonExperiments

