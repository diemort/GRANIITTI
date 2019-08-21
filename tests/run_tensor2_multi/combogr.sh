#!/bin/sh

# weighted events for speed

N=10000

./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:1.0, g1:0.0, g2:0.0, g3:0.0, g4:0.0, g5:0.0, g6:0.0}" -w true -l false -n $N -o "f2_0" -f "hepmc3"
./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:0.0, g1:1.0, g2:0.0, g3:0.0, g4:0.0, g5:0.0, g6:0.0}" -w true -l false -n $N -o "f2_1" -f "hepmc3"
./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:0.0, g1:0.0, g2:1.0, g3:0.0, g4:0.0, g5:0.0, g6:0.0}" -w true -l false -n $N -o "f2_2" -f "hepmc3"
./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:0.0, g1:0.0, g2:0.0, g3:1.0, g4:0.0, g5:0.0, g6:0.0}" -w true -l false -n $N -o "f2_3" -f "hepmc3"
./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:0.0, g1:0.0, g2:0.0, g3:0.0, g4:1.0, g5:0.0, g6:0.0}" -w true -l false -n $N -o "f2_4" -f "hepmc3"
./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:0.0, g1:0.0, g2:0.0, g3:0.0, g4:0.0, g5:1.0, g6:0.0}" -w true -l false -n $N -o "f2_5" -f "hepmc3"
./bin/gr -i ./tests/processes/CMS13.json -p "PP[RESTENSOR]<F> -> pi+ pi- @RES{f2_1270:1} @R[f2_1270]{g0:0.0, g1:0.0, g2:0.0, g3:0.0, g4:0.0, g5:0.0, g6:1.0}" -w true -l false -n $N -o "f2_6" -f "hepmc3"