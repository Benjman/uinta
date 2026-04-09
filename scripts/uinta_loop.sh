#!/usr/bin/env bash

DIR=$(git rev-parse --show-toplevel)
DIR="${DIR:-$(git rev-parse --show-toplevel)}"

[[ ! -d $DIR ]] && echo "$DIR not found" && exit

while :; do
  cmake -B $DIR/build $DIR || exit $?
  make -j$(nproc) -C $DIR/build || exit $?
  echo ":: Sleeping ${SLEEP:-1} sec..." && sleep ${SLEEP:-1}
  $DIR/build/src/app/app "$@" || exit $?
done
