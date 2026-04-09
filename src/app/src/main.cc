#include <absl/log/log.h>
#include <absl/strings/str_format.h>

#include <iostream>

#include "uinta/engine/engine.h"

int main() {
  std::cerr << uinta::Engine().Message();
  return 0;
}
