#!  /bin/bash

for i in `seq 1 $1`
do
    echo "$RANDOM $2 % p" | dc
done

# vim: ts=4 sw=4 et
