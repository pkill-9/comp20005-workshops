#!  /bin/bash
#
# Script to generate some random numbers. It should have 3 command line
# arguments: the first two are the lower and upper limits of the numbers
# to generate, and the third is the number of randoms to be generated.
#
# eg: ./random-numbers.sh 4 107 50
# 
# will generate 50 random numbers between 4 and 107 inclusive.


for i in `seq 1 $3`
do
    # you are not expected to understand this.
    echo "$RANDOM $2 $1 - % $1 + p" | dc
done

# vim: ts=4 sw=4 et
