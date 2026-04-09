#!/bin/zsh

DIR=$(git rev-parse --show-toplevel)
SRC_DIR="${SRC_DIR:-$DIR}"
WATCH_DIR="${WATCH_DIR:-$DIR/src}"
BUILD_DIR="${BUILD_DIR:-$SRC_DIR/build}"

if ! command -v inotifywait &>/dev/null; then
  yay -S inotify-tools
fi

is_git_mid_operation() {
  local git_dir="$DIR/.git"
  [[ -d "$git_dir/rebase-merge" ]] || \
  [[ -d "$git_dir/rebase-apply" ]] || \
  [[ -f "$git_dir/MERGE_HEAD" ]] || \
  [[ -f "$git_dir/BISECT_LOG" ]]
}

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

GIT_DIR="$DIR/.git"
was_mid_operation=false

try_build() {
  if is_git_mid_operation; then
    if [[ "$was_mid_operation" != true ]]; then
      echo "Skipping build: git operation in progress (rebase/merge/bisect)"
    fi
    was_mid_operation=true
    return 0
  fi

  if [[ "$was_mid_operation" == true ]]; then
    echo "Git operation completed, triggering build..."
    was_mid_operation=false
    build_project
  else
    build_project
  fi
}

try_build

while true; do
  if inotifywait -qre 'modify,create,delete,move' "$WATCH_DIR" "$GIT_DIR"; then
    try_build
  fi
done
