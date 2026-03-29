#ifndef SRC_ENGINE_INCLUDE_UINTA_LUA_CORE_BINDINGS_H_
#define SRC_ENGINE_INCLUDE_UINTA_LUA_CORE_BINDINGS_H_

#include "uinta/lua/binding_module.h"

namespace uinta {

class Engine;

/// Provides core engine bindings to Lua scripts.
/// Includes: logging, math types (vec2, vec3, vec4), and engine info.
class CoreBindings : public BindingModule {
 public:
  /// Construct core bindings.
  /// @param engine Pointer to the engine for accessing runtime info.
  explicit CoreBindings(const Engine* engine = nullptr) noexcept;

  void registerBindings(sol::state& lua) override;

 private:
  const Engine* engine_;

  void registerLogging(sol::state& lua);
  void registerMathTypes(sol::state& lua);
  void registerEngineInfo(sol::state& lua);
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_LUA_CORE_BINDINGS_H_
