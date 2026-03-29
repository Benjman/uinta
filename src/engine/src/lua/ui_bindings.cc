#include "uinta/lua/ui_bindings.h"

#include <absl/log/log.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <sol/sol.hpp>  // IWYU pragma: keep
#pragma GCC diagnostic pop

namespace uinta {

void UIBindings::registerBindings(sol::state& lua) {
  // Create uinta.ui namespace as a stub
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table ui = uinta["ui"].get_or_create<sol::table>();

  // Stub functions that log a warning
  auto stubWarning = [](const std::string& funcName) {
    LOG(WARNING) << "[Lua] uinta.ui." << funcName
                 << " is not yet implemented (UI bindings are stubbed)";
  };

  // Placeholder widget creation functions
  ui["createWindow"] = [stubWarning](const std::string& /*title*/) {
    stubWarning("createWindow");
    return sol::nil;
  };

  ui["createButton"] = [stubWarning](const std::string& /*label*/) {
    stubWarning("createButton");
    return sol::nil;
  };

  ui["createLabel"] = [stubWarning](const std::string& /*text*/) {
    stubWarning("createLabel");
    return sol::nil;
  };

  ui["createPanel"] = [stubWarning]() {
    stubWarning("createPanel");
    return sol::nil;
  };

  // Placeholder for future implementation note
  ui["_stub_notice"] = "UI bindings are stubbed. Full implementation TBD.";
}

}  // namespace uinta
