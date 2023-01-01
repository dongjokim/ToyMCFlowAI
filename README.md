# ToyMCFlow2022
To run:

make clean

make

./ToyMcFlowAO outputFile Nevt random_seed bgfrac bNUE

e.g. ./ToyMCFlowAO "outputs/10kEv50BG1HoleSampF.root" 10000 11 0.5 1
  
The parameters given to the code in the previous line are the output filename, Number of events, a seed, the background fraction and whether or not the non-uniform distr is used

More parameters can be set in the code ToyMCFlowAO.C itself, remember to save and "make" after changing those!

Changing the setting to include two particle azim. method on will significantly slow the run!
