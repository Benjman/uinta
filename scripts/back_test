#!/bin/bash

#
# This script is intended to move forward from a git rebase position, running
# all available tests. If any compilation fails, or any tests failed, the script
# will abort.
#

DIR=$(git rev-parse --show-toplevel)
BUILD_DIR="$DIR/build"

in_rebase_state() {
  [ -d "$DIR/.git/rebase-apply" ] || [ -d "$DIR/.git/rebase-merge" ]
}

is_edit_step() {
  if [ -d "$DIR/.git/rebase-apply" ]; then
    [ -f "$DIR/.git/rebase-apply/patch" ]
  elif [ -d "$DIR/.git/rebase-merge" ]; then
    [ -f "$DIR/.git/rebase-merge/msgnum" ]
  else
    return 1
  fi
}

continue_rebase() {
  echo "Continuing rebase..."
  git rebase --continue
}

while in_rebase_state; do
  if is_edit_step; then

    cmake -S $DIR -B $BUILD_DIR || exit $?
    make -j$(nproc) --directory $BUILD_DIR || exit $?
    $BUILD_DIR/src/engine/test/engine_test || exit $?
    $BUILD_DIR/src/platform/test/platform_test || exit $?

    continue_rebase
    if [ $? -ne 0 ]; then
      echo "Rebase failed. Exiting."
      exit 1
    fi
  else
    echo "Not in an 'edit' step. Exiting."
    exit 1
  fi
done

echo "Rebase completed successfully."
