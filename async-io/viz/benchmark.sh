#!/usr/bin/env bash

if [ $# != 3 ]; then
    printf 'Invalid parameter count. Provide Name and Both executables"\n'
    exit 1;
fi

function generate()
{
  rep_count=5
  printf "{\n\
  \"name\": \"${4}\",\n\
  \"runs\": [\n" >> $3
  for i in $(seq 1 $rep_count)
  do
    /usr/bin/time --format="{\n\
    \"kernel\": %S,\n\
    \"user\": %U,\n\
    \"memory\": %M\n\
    }"\
     --output="${3}" --append ./${2} ${1} > /dev/null

    printf $([ "$i" != "$rep_count" ] && echo ",\n" || echo "\n") >> $3;
  done
  printf "]\n}" >> $3
}

function benchmark()
{
  printf "{\n\
  \"name\": \"${5}\",\n\
  \"versions\": [\n" >> $4
  generate $1 $2 $4 "Serial"
  printf ",\n" >> $4
  generate $1 $3 $4 "Async"
  printf "\n]\n}\n" >> $4
}

NAME="$1"
SERIAL="$2"
ASYNC="$3"

RESULT_FILE="result.json"

benchmark 10000000 $SERIAL $ASYNC $RESULT_FILE $NAME
