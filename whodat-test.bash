#!/bin/bash

FAILS=0

run_test() {
    ./whodat "$@" > wilsmith_solution.txt
    
    cd Lab1 || exit 1
    ./whodat "$@" > /tmp/rchaney_output.txt
    cd ..

    #Will test the two solutions after the output from expect directory was 
    #moved to directory of my solution.
    if diff -y wilsmith_solution.txt /tmp/rchaney_output.txt > /dev/null;
    then
         echo "Pass"
    else
         echo "Fail"
         (( FAILS++ ))
    fi
}
    
#Test cases
run_test rchaney -l

run_test -v rchaney

run_test -p rchaney

run_test -P rchaney

run_test rchaney dmcgrath -l

run_test rchaney -Y

run_test rchaney -L 5

run_test -q

run_test adamsmith rchaney

run_test -l -p -P rchaney dmcgrath mpj

run_test mpj -Ppvl rchaney -v

run_test rchaney -vvP mpj -L5 dmcgrath -pv

run_test rchaney mpj dmcgrath -L 3 -p

run_test rchaney -h

run_test rchaney dmcgrath -h

run_test

run_test rchaney karlaf dmcgrath qsmith

run_test rchaney dmcgrath -L 4

run_test JUNK rchaney

run_test rchaney -L4 dmcgrath -L5

run_test wilsmith rchaney -Y

run_test dmcgrath rchaney -h

run_test rchaney -h dmcgrath

run_test -h rchaney dmcgrath

rm wilsmith_solution.txt /tmp/rchaney_output.txt

if [ ${FAILS} -eq 0 ]
then
    echo "looks good"
else
    echo "check code for correctness"
fi





