#!/usr/bin/env bash

if [ $# != 2 ]; then
    printf 'Invalid parameter count. Provide Both executables"\n'
    exit 1;
fi

function benchmark()
{
  #for i in {1..10}
  #do
    for test_size in 1000 100000 10000000
    do
      printf "Test size: ${test_size}\n"

      printf "\nTime A:"
      time ./${1} $test_size > /dev/null

      printf "\nTime B:"
      time ./${2} $test_size > /dev/null
      printf "\n-----\n\n"

    done
  #done
}

SERIAL="$1"
ASYNC="$2"

benchmark $SERIAL $ASYNC
