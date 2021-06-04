#!/bin/bash

function check_files {
    for i do
        if [ ! -f $i ]; then
            return 1
        fi
    done
}

function check_compile {
    x=1
    cd $1
    gcc -std=c99 -Wall -Wextra *.c -o exTest &> /dev/null
    #gcc -std=c99 -Wall -Wextra *.c -o exTest
    x=$?
    cd ../
    [[ $x -ne 0 ]]
}

function run_tests {
    tests=("test1" "test2")
    cd $1
    #get the makefile to produce "exTest"
    gcc -std=c99 -Wall -Wextra *.c -o exTest &> /dev/null
    for test in "${tests[@]}"; do
        #./exTest < $test.in | diff $test.out - 
        ./exTest < $test.in | diff $test.out - &> /dev/null
        res=$?

        if [[ res -ne 0 ]]; then
            echo "Test $test.in failed"
            break
        fi
    done
    cd ../
}

function transfer_files {
    files=(
        "test1.in"
        "test1.out"
        "test2.in"
        "test2.out"
    )

    for file in "${files[@]}"; do
        cp $1/$2/$3/$file $3
    done
}

echo "Checking zip file...."

if [ "$#" -ne 1 ]; then
    echo "You should provide your zip file as a parameter and nothing else!"
    exit 1
fi

if ! [[ $1 =~ ^A[0-9]{7}[A-Z].zip$ ]]; then
    echo "zip file is wrongly named: it should be <STUDNET ID>.zip"
    exit 1
fi

echo "Unzipping file: $1"
if [ ! -f $1 ]; then
    echo "File $1 does not exist."
    exit 1
fi

tmp_folder="test_grading_aeN332Hp"
rm -rf $tmp_folder
mkdir $tmp_folder

cp $1 $tmp_folder

cd $tmp_folder
unzip $1 > /dev/null

exercises=(ex3)
ex3=("ex3/ex3.c")
for ex in "${exercises[@]}"; do
    declare -n files=$ex
    if [ ! -d $ex ]
    then
        echo "$ex: Failed - $ex folder missing"
        continue
    fi

    if ! check_files "${files[@]}"
    then
        echo "$ex: Failed - files missing"
        continue
    fi

    if check_compile $ex
    then
        echo "$ex: Failed - does not compile"
        continue
    fi
    echo "$ex: Success"
done

cd ../
rm -rf $tmp_folder
