#!  /bin/bash
#
# Shell script to test our change calculator exhaustively on every input
# between 0 and 99 cents.

separator="=============================="

# clear any errors recorded in previous runs.
rm -f errors
touch errors

for ((i = 0; i <= 99; i ++))
do
    echo $i | ./listchange > output

    # check the status returned by the program. If it is non zero, record
    # the output as an error, to be displayed later.
    #
    # Note that the status is in the shell variable $?, not $status as may
    # be used on OSX.
    if test $? != 0
    then
        echo ${separator} >> errors
        echo "Input: $i cents" >> errors
        cat output >> errors
    fi
done

# were there any errors? If the error file is of non zero length, there
# must have been errors.
if test -s errors
then
    echo "Errors occurred. Output logged in \"errors\"."
else
    echo "No errors :)"
fi

# vim: ts=4 sw=4 et
