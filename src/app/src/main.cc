#include <iostream>

#include "uinta/engine/engine.h"

int main() {
  const auto message = uinta::Engine().Message();
  if (!message.ok())
    std::cerr << message.status();
  else
    std::cout << message;
  return 0;
}
