#!/bin/bash

#
# This script is intended to move forward from a git rebase position, running
# all available tests. If any compilation fails, or any tests failed, the script
# will abort.
#

DIR=$(git rev-parse --show-toplevel)
BUILD_DIR="$DIR/build"

REBASE_APPLY_DIR=$(git rev-parse --git-path rebase-apply)
REBASE_MERGE_DIR=$(git rev-parse --git-path rebase-merge)

in_rebase_state() {
  [ -d "$REBASE_APPLY_DIR" ] || [ -d "$REBASE_MERGE_DIR" ]
}

is_edit_step() {
  if [ -d "$REBASE_APPLY_DIR" ]; then
    [ -f "$REBASE_APPLY_DIR/patch" ]
  elif [ -d "$REBASE_MERGE_DIR" ]; then
    [ -f "$REBASE_MERGE_DIR/msgnum" ]
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
