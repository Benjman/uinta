#include "uinta/localization/localization_system.h"

#include <gtest/gtest.h>

#include "./utils.h"
#include "uinta/engine/engine.h"
#include "uinta/engine/service_registry.h"
#include "uinta/gl.h"
#include "uinta/localization/locale.h"
#include "uinta/localization/localization.h"
#include "uinta/mock/mock_gl.h"
#include "uinta/mock/mock_platform.h"

namespace uinta {

class LocalizationSystemTest : public UintaTestF {
 protected:
  LocalizationSystemTest() noexcept { serviceRegistry_.registerService<const OpenGLApi>(&gl_); }

  ServiceRegistry serviceRegistry_;
  MockOpenGLApi gl_;
};

TEST_F(LocalizationSystemTest, LoadsEnUsOnConstruction) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  const auto* localization = engine.service<LocalizationSystem>();
  ASSERT_NE(nullptr, localization);
  EXPECT_EQ(Locale::EnUs, localization->locale());
  EXPECT_EQ("Hello localization!", localization->getString(Localization::HelloLocalization));
}

TEST_F(LocalizationSystemTest, DiscoverableAsService) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  EXPECT_NE(nullptr, engine.service<LocalizationSystem>());
}

TEST_F(LocalizationSystemTest, LocaleAccessorMatchesEngineParams) {
  MockPlatform platform;
  auto engine = makeEngine(&platform);

  const auto* localization = engine.service<LocalizationSystem>();
  ASSERT_NE(nullptr, localization);
  EXPECT_EQ(Locale::EnUs, localization->locale());
}

TEST_F(LocalizationSystemTest, ToStringAndToLocaleRoundtrip) {
  EXPECT_EQ("en-us", toString(Locale::EnUs));
  EXPECT_EQ("unknown", toString(Locale::Unknown));
  EXPECT_EQ(Locale::EnUs, toLocale("en-us"));
  EXPECT_EQ(Locale::Unknown, toLocale("nonsense"));
}

TEST_F(LocalizationSystemTest, ToKeyMapsKnownEnumValue) {
  EXPECT_EQ("hello_localization", toKey(Localization::HelloLocalization));
}

}  // namespace uinta
