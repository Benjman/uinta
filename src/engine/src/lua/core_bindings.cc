#include "uinta/lua/core_bindings.h"

#include <absl/log/log.h>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <sol/sol.hpp>  // IWYU pragma: keep
#pragma GCC diagnostic pop

#include "uinta/engine/engine.h"
#include "uinta/engine/engine_state.h"

namespace uinta {

CoreBindings::CoreBindings(const Engine* engine) noexcept : engine_(engine) {}

void CoreBindings::registerBindings(sol::state& lua) {
  registerLogging(lua);
  registerMathTypes(lua);
  registerEngineInfo(lua);
}

void CoreBindings::registerLogging(sol::state& lua) {
  // Create uinta.log namespace
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table log = uinta["log"].get_or_create<sol::table>();

  log["info"] = [](const std::string& msg) { LOG(INFO) << "[Lua] " << msg; };

  log["warn"] = [](const std::string& msg) { LOG(WARNING) << "[Lua] " << msg; };

  log["error"] = [](const std::string& msg) { LOG(ERROR) << "[Lua] " << msg; };

#ifdef UINTA_DEBUG
  log["debug"] = [](const std::string& msg) {
    LOG(INFO) << "[Lua/Debug] " << msg;
  };
#else
  // In release builds, debug logging is a no-op
  log["debug"] = [](const std::string& /*unused*/) {};
#endif
}

void CoreBindings::registerMathTypes(sol::state& lua) {
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();

  // vec2
  sol::usertype<glm::vec2> vec2_type = lua.new_usertype<glm::vec2>(
      "vec2",
      sol::constructors<glm::vec2(), glm::vec2(float),
                        glm::vec2(float, float)>(),
      "x", &glm::vec2::x, "y", &glm::vec2::y, sol::meta_function::addition,
      [](const glm::vec2& a, const glm::vec2& b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec2& a, const glm::vec2& b) { return a - b; },
      sol::meta_function::multiplication,
      sol::overload(
          [](const glm::vec2& a, float b) { return a * b; },
          [](float a, const glm::vec2& b) { return a * b; },
          [](const glm::vec2& a, const glm::vec2& b) { return a * b; }),
      sol::meta_function::division,
      sol::overload(
          [](const glm::vec2& a, float b) { return a / b; },
          [](const glm::vec2& a, const glm::vec2& b) { return a / b; }),
      sol::meta_function::unary_minus, [](const glm::vec2& v) { return -v; },
      sol::meta_function::equal_to,
      [](const glm::vec2& a, const glm::vec2& b) { return a == b; },
      sol::meta_function::to_string,
      [](const glm::vec2& v) {
        return "vec2(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
      },
      "length", [](const glm::vec2& v) { return glm::length(v); }, "normalize",
      [](const glm::vec2& v) { return glm::normalize(v); }, "dot",
      [](const glm::vec2& a, const glm::vec2& b) { return glm::dot(a, b); });

  uinta["vec2"] = [](float x, float y) { return glm::vec2(x, y); };

  // vec3
  sol::usertype<glm::vec3> vec3_type = lua.new_usertype<glm::vec3>(
      "vec3",
      sol::constructors<glm::vec3(), glm::vec3(float),
                        glm::vec3(float, float, float)>(),
      "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z,
      sol::meta_function::addition,
      [](const glm::vec3& a, const glm::vec3& b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec3& a, const glm::vec3& b) { return a - b; },
      sol::meta_function::multiplication,
      sol::overload(
          [](const glm::vec3& a, float b) { return a * b; },
          [](float a, const glm::vec3& b) { return a * b; },
          [](const glm::vec3& a, const glm::vec3& b) { return a * b; }),
      sol::meta_function::division,
      sol::overload(
          [](const glm::vec3& a, float b) { return a / b; },
          [](const glm::vec3& a, const glm::vec3& b) { return a / b; }),
      sol::meta_function::unary_minus, [](const glm::vec3& v) { return -v; },
      sol::meta_function::equal_to,
      [](const glm::vec3& a, const glm::vec3& b) { return a == b; },
      sol::meta_function::to_string,
      [](const glm::vec3& v) {
        return "vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) +
               ", " + std::to_string(v.z) + ")";
      },
      "length", [](const glm::vec3& v) { return glm::length(v); }, "normalize",
      [](const glm::vec3& v) { return glm::normalize(v); }, "dot",
      [](const glm::vec3& a, const glm::vec3& b) { return glm::dot(a, b); },
      "cross",
      [](const glm::vec3& a, const glm::vec3& b) { return glm::cross(a, b); });

  uinta["vec3"] = [](float x, float y, float z) { return glm::vec3(x, y, z); };

  // vec4
  sol::usertype<glm::vec4> vec4_type = lua.new_usertype<glm::vec4>(
      "vec4",
      sol::constructors<glm::vec4(), glm::vec4(float),
                        glm::vec4(float, float, float, float)>(),
      "x", &glm::vec4::x, "y", &glm::vec4::y, "z", &glm::vec4::z, "w",
      &glm::vec4::w, sol::meta_function::addition,
      [](const glm::vec4& a, const glm::vec4& b) { return a + b; },
      sol::meta_function::subtraction,
      [](const glm::vec4& a, const glm::vec4& b) { return a - b; },
      sol::meta_function::multiplication,
      sol::overload(
          [](const glm::vec4& a, float b) { return a * b; },
          [](float a, const glm::vec4& b) { return a * b; },
          [](const glm::vec4& a, const glm::vec4& b) { return a * b; }),
      sol::meta_function::division,
      sol::overload(
          [](const glm::vec4& a, float b) { return a / b; },
          [](const glm::vec4& a, const glm::vec4& b) { return a / b; }),
      sol::meta_function::unary_minus, [](const glm::vec4& v) { return -v; },
      sol::meta_function::equal_to,
      [](const glm::vec4& a, const glm::vec4& b) { return a == b; },
      sol::meta_function::to_string,
      [](const glm::vec4& v) {
        return "vec4(" + std::to_string(v.x) + ", " + std::to_string(v.y) +
               ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
      },
      "length", [](const glm::vec4& v) { return glm::length(v); }, "normalize",
      [](const glm::vec4& v) { return glm::normalize(v); }, "dot",
      [](const glm::vec4& a, const glm::vec4& b) { return glm::dot(a, b); });

  uinta["vec4"] = [](float x, float y, float z, float w) {
    return glm::vec4(x, y, z, w);
  };
}

void CoreBindings::registerEngineInfo(sol::state& lua) {
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table engine = uinta["engine"].get_or_create<sol::table>();

  if (engine_ != nullptr) {
    // FrameManager::frequency is the target frame rate (FPS)
    engine["fps"] = sol::property([this]() -> float {
      if (engine_) {
        return engine_->frameManager().frequency;
      }
      return 0.0f;
    });

    engine["deltaTime"] = sol::property([this]() -> float {
      if (engine_) {
        return static_cast<float>(engine_->state().delta());
      }
      return 0.0f;
    });

    engine["runtime"] = sol::property([this]() -> float {
      if (engine_) {
        return static_cast<float>(engine_->state().runtime());
      }
      return 0.0f;
    });
  } else {
    // Fallback when no engine is available
    engine["fps"] = 0.0f;
    engine["deltaTime"] = 0.0f;
    engine["runtime"] = 0.0f;
  }
}

}  // namespace uinta
