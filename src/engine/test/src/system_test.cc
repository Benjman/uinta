#include "uinta/system.h"

#include <gtest/gtest.h>

namespace uinta {

TEST(SystemTest, OneSystemTypePerCollection) {
  // TODO:
  class TestSystem : public System {
   public:
  };
}

}  // namespace uinta
