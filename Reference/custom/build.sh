#!/usr/bin/env bash

function build()
{
  clang++ -O3 -Wall -Wextra -std=c++1z -stdlib=libc++ -pthread -I ./ \
  "${1}" -o "${2}"
}

build serial.cpp serial.out
build async.cpp async.out

# ./test.sh serial.out async.out
