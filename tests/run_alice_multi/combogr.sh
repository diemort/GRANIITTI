#!/bin/sh

# weighted events for speed

./bin/gr -i ./tests/processes/ALICE_2pi.json -w true -l false -n 100000
./bin/gr -i ./tests/processes/ALICE_2K.json -w true -l false -n 100000
./bin/gr -i ./tests/processes/ALICE_ppbar.json -w true -l false -n 100000
