#!/usr/bin/env bash

if [ $# != 1 ]; then
    printf 'Invalid parameter count. Provide Both executables"\n'
    exit 1;
fi

function createRef()
{
  for (( i=0; i<$1; i++ ))
  do
    printf "$i"
  done
}

function test()
{
  test_size=$2
  printf "Test size: ${test_size}\n"

  tmp_file_a="tmp_file_a"
  tmp_file_b="tmp_file_b"

  createRef $test_size > $tmp_file_a
  ./${1} $test_size > $tmp_file_b

  # Diff binary, requires xxd
  #res=$(diff <(xxd $tmp_file_a) <(xxd $tmp_file_b))

  res=$(diff $tmp_file_a $tmp_file_b)
  rm $tmp_file_a $tmp_file_b

  if [ -n "$res" ]; then
    printf "Diff:\n${res}\n"
    printf "Test FAILED\n"
    exit 1;
  fi

  printf "Test SUCCEDED\n"
}

for test_size in 10 1000 100000
do
  test $1 $test_size
done
