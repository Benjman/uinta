#!/bin/zsh

DIR=$(git rev-parse --show-toplevel)
SRC_DIR="${SRC_DIR:-$DIR}"
WATCH_DIR="${WATCH_DIR:-$DIR/src}"
BUILD_DIR="${BUILD_DIR:-$SRC_DIR/build}"

if ! command -v inotifywait &>/dev/null; then
  yay -S inotify-tools
fi

build_project() {
  reset

  local date_str="Built on $(date)"
  local separator_len=$((COLUMNS - ${#date_str} - 1))
  printf '%*s %s\n' "$separator_len" '' "$date_str" | tr ' ' -
  cmake -S "$SRC_DIR" -B "$BUILD_DIR"
  make -j$(nproc) --directory "$BUILD_DIR" || return $?
  if (( ${+EXEC_POST} )); then
    eval $EXEC_POST & echo "PID: $!"
  fi
}

build_project

while true; do
  if inotifywait -qre modify "$WATCH_DIR"; then
    build_project
  fi
done
