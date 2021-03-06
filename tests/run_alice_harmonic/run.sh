#!/bin/sh
#
# Simulation and spherical harmonic expansion with ALICE cuts
#
# Run with: source ./tests/run_xxx/run.sh

read -p "run: Generate events (or only analyze)? [y/n] " -n 1 -r
echo # New line

if [[ $REPLY =~ ^[Yy]$ ]]
then

# Not set
if [ -z "$EVENTS" ]
then
	EVENTS=1000000
fi

# Generate
./bin/gr -i ./tests/run_alice_harmonic/SH_2pi_J0_ALICE.json -n $EVENTS
./bin/gr -i ./tests/run_alice_harmonic/SH_2pi_ALICE.json    -n $EVENTS

fi

# ***********************************************************************
# Fiducial cuts <ETAMIN,ETAMAX,PTMIN,PTMAX> (no white space!)
FIDCUTS=-0.9,0.9,0.15,100.0
# ***********************************************************************

# System kinematic variables binning <bins,min,max> (no white space!)
MBINS=30,0.28,2.0
PBINS=1,0.0,10.0
YBINS=1,-0.9,0.9

# PARAMETERS
LMAX=1
REMOVEODD=true
REMOVENEGATIVE=true
SVDREG=1e-5
L1REG=0
EML=false

# Lorentz frames
for FRAME in AH CM HX CS PG GJ
do

# Expand the data
./bin/fitharmonic \
-r "SH_2pi_J0_ALICE" \
-i "SH_2pi_J0_ALICE, SH_2pi_ALICE, ALICE7_2pi.csv" \
-l "GRANIITTI J=0, GRANIITTI, #pi^{+}#pi^{-} 7 TeV (DATA)" \
-d "MC,MC,DATA" \
-z "true,true,false"  \
-t "#Omega{Detector}: |#eta| < 0.9 #wedge p_{T} > 0.15 GeV,#Omega{Fiducial}: |#eta| < 0.9 #wedge p_{T} > 0.15 GeV,#Omega{Flat}: |Y_{x}| < 0.9" \
-c $FIDCUTS \
-f $FRAME -g $LMAX -o $REMOVEODD -v $REMOVENEGATIVE -a $SVDREG -b $L1REG -e $EML \
-M $MBINS -P $PBINS -Y $YBINS \
-S "-1.0, -1.0, -1.0" \
-w "" \
-X 100000000

# -S -1 for normalized rates

done

# Implement 2D harmonic plots (M,Pt)
# ...