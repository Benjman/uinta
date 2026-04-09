#!/bin/bash
set -e

# Simple coverage generator for nvim-coverage
# Generates lcov.info for C++
# Usage: ./coverage.sh [--html]

BUILD_DIR="build"
GENERATE_HTML=false

# Parse arguments
if [[ "$1" == "--html" ]]; then
  GENERATE_HTML=true
fi

# Coverage instrumentation is opt-in (see CMakeLists.txt), since it makes
# installed libraries unlinkable by consumers that don't also build with
# --coverage. Always (re)configure with it on so this script works
# regardless of how $BUILD_DIR was last configured.
cmake -B "$BUILD_DIR" . -DCMAKE_BUILD_TYPE=Debug -DUINTA_ENABLE_COVERAGE=ON
cmake --build "$BUILD_DIR" -j"$(nproc)"

echo "Running tests to generate coverage data..."
find "$BUILD_DIR" -name "*.gcda" -delete
"$BUILD_DIR/src/platform/test/platform_test"
"$BUILD_DIR/src/engine/test/engine_test"

# Generate lcov.info
lcov --capture \
  --directory "$BUILD_DIR" \
  --output-file "lcov.info" \
  --rc branch_coverage=1 \
  --ignore-errors mismatch,inconsistent,unused,negative \
  2>/dev/null

# Remove external code
lcov --remove "lcov.info" \
  '*/lib/*' \
  '*/test/*' \
  '/usr/*' \
  --output-file "lcov.info" \
  --rc branch_coverage=1 \
  --ignore-errors mismatch,inconsistent,unused,negative \
  2>/dev/null

echo "Coverage data generated: lcov.info"
lcov --summary "lcov.info" 2>&1 | grep -E "lines|functions|branches"

# Generate HTML report if requested
if [ "$GENERATE_HTML" = true ]; then
  echo ""
  echo "Generating HTML report..."
  genhtml "lcov.info" \
    --output-directory "coverage_html" \
    --title "Uinta Coverage Report" \
    --legend \
    --rc branch_coverage=1 \
    --ignore-errors inconsistent,source,corrupt \
    2>/dev/null

  echo "HTML report generated: coverage_html/index.html"
fi

