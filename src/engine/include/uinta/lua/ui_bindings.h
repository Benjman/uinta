#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_UI_BINDINGS_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_UI_BINDINGS_H_

#include "uinta/lua/binding_module.h"

namespace uinta {

/// Stub UI bindings module.
/// Provides placeholder API for future UI widget implementation.
/// The actual design is TBD - this just registers empty namespaces.
class UIBindings : public BindingModule {
 public:
  UIBindings() = default;

  void registerBindings(sol::state& lua) override;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_UI_BINDINGS_H_
