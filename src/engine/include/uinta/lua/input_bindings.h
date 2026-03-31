#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_INPUT_BINDINGS_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_INPUT_BINDINGS_H_

#include <vector>

#include "uinta/input/input_token.h"
#include "uinta/lua/binding_module.h"

namespace uinta {

class Engine;

/// Lua bindings for the InputSystem.
/// Exposes keyboard, mouse button, mouse movement, and scroll event
/// subscriptions to Lua plugins with automatic cleanup on reload.
class InputBindings : public BindingModule {
 public:
  explicit InputBindings(Engine* engine) noexcept;

  void registerBindings(sol::state& lua) override;
  void onBeforeReload() override;

 private:
  Engine* engine_;
  std::vector<SubscriptionHandle> activeHandles_;

  void registerEnums(sol::state& lua);
  void registerInputFunctions(sol::state& lua);
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_INPUT_BINDINGS_H_
