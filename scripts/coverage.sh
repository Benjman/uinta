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

# Run tests if .gcda files don't exist
if ! find "$BUILD_DIR" -name "*.gcda" 2>/dev/null | grep -q .; then
  echo "Running tests to generate coverage data..."
  "$BUILD_DIR/src/platform/test/platform_test"
  "$BUILD_DIR/src/engine/test/engine_test"
fi

# Generate lcov.info
lcov --capture \
  --directory "$BUILD_DIR" \
  --output-file "lcov.info" \
  --rc branch_coverage=1 \
  --ignore-errors mismatch,inconsistent,unused \
  2>/dev/null

# Remove external code
lcov --remove "lcov.info" \
  '*/lib/*' \
  '*/test/*' \
  '/usr/*' \
  --output-file "lcov.info" \
  --rc branch_coverage=1 \
  --ignore-errors mismatch,inconsistent,unused \
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

