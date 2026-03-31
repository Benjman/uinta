#include "uinta/lua/core_bindings.h"

#include <absl/log/log.h>

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/geometric.hpp>

#include "uinta/math/smooth_float.h"
#include "uinta/math/smooth_vec.h"

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
  registerMathExtensions(lua);
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

void CoreBindings::registerMathExtensions(sol::state& lua) {
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();

  // SmoothFloat
  lua.new_usertype<SmoothFloat>(
      "SmoothFloat", sol::constructors<SmoothFloat(float, float)>(), "target",
      sol::overload([](const SmoothFloat& sf) { return sf.target(); },
                    [](SmoothFloat& sf, float v) { sf.target(v); }),
      "current",
      sol::overload([](const SmoothFloat& sf) { return sf.current(); },
                    [](SmoothFloat& sf, float v) { sf.current(v); }),
      "update", &SmoothFloat::update, "force",
      sol::overload([](SmoothFloat& sf) { sf.force(); },
                    [](SmoothFloat& sf, float v) { sf.force(v); }),
      "agility", [](const SmoothFloat& sf) { return sf.agility(); },
      "setAgility", [](SmoothFloat& sf, float v) { sf.agility(v); },
      sol::meta_function::to_string,
      [](const SmoothFloat& sf) {
        return "SmoothFloat(current=" + std::to_string(sf.current()) +
               ", target=" + std::to_string(sf.target()) + ")";
      });

  uinta["SmoothFloat"] = [](float agility, float initial) {
    return SmoothFloat(agility, initial);
  };

  // SmoothVec2
  lua.new_usertype<SmoothVec2>(
      "SmoothVec2",
      sol::constructors<SmoothVec2(float, float, float),
                        SmoothVec2(float, const glm::vec2&)>(),
      "target",
      sol::overload(
          [](const SmoothVec2& sv) {
            return glm::vec2(sv.x().target(), sv.y().target());
          },
          [](SmoothVec2& sv, const glm::vec2& v) { sv = v; }),
      "current",
      sol::overload(
          [](const SmoothVec2& sv) { return static_cast<glm::vec2>(sv); },
          [](SmoothVec2& sv, const glm::vec2& v) {
            SmoothVec2 temp(sv.x().agility(), v);
            temp.force();
            sv = temp;
          }),
      "update", &SmoothVec2::update, "force",
      sol::overload([](SmoothVec2& sv) { sv.force(); },
                    [](SmoothVec2& sv, const glm::vec2& v) { sv.force(v); }),
      "agility", [](const SmoothVec2& sv) { return sv.x().agility(); },
      "setAgility", [](SmoothVec2& sv, float v) { sv.agility(v); },
      sol::meta_function::to_string,
      [](const SmoothVec2& sv) {
        glm::vec2 c = sv;
        return "SmoothVec2(current=(" + std::to_string(c.x) + ", " +
               std::to_string(c.y) + "))";
      });

  uinta["SmoothVec2"] = sol::overload(
      [](float agility, const glm::vec2& v) { return SmoothVec2(agility, v); },
      [](float agility, float x, float y) {
        return SmoothVec2(agility, x, y);
      });

  // SmoothVec3
  lua.new_usertype<SmoothVec3>(
      "SmoothVec3",
      sol::constructors<SmoothVec3(float, float, float, float),
                        SmoothVec3(float, const glm::vec3&)>(),
      "target",
      sol::overload(
          [](const SmoothVec3& sv) {
            return glm::vec3(sv.x().target(), sv.y().target(), sv.z().target());
          },
          [](SmoothVec3& sv, const glm::vec3& v) { sv = v; }),
      "current",
      sol::overload(
          [](const SmoothVec3& sv) { return static_cast<glm::vec3>(sv); },
          [](SmoothVec3& sv, const glm::vec3& v) {
            SmoothVec3 temp(sv.x().agility(), v);
            temp.force();
            sv = temp;
          }),
      "update", &SmoothVec3::update, "force",
      sol::overload([](SmoothVec3& sv) { sv.force(); },
                    [](SmoothVec3& sv, const glm::vec3& v) { sv.force(v); }),
      "agility", [](const SmoothVec3& sv) { return sv.x().agility(); },
      "setAgility", [](SmoothVec3& sv, float v) { sv.agility(v); },
      sol::meta_function::to_string,
      [](const SmoothVec3& sv) {
        glm::vec3 c = sv;
        return "SmoothVec3(current=(" + std::to_string(c.x) + ", " +
               std::to_string(c.y) + ", " + std::to_string(c.z) + "))";
      });

  uinta["SmoothVec3"] = sol::overload(
      [](float agility, const glm::vec3& v) { return SmoothVec3(agility, v); },
      [](float agility, float x, float y, float z) {
        return SmoothVec3(agility, x, y, z);
      });

  // mat4
  sol::usertype<glm::mat4> mat4_type = lua.new_usertype<glm::mat4>(
      "mat4", sol::constructors<glm::mat4()>(),
      sol::meta_function::multiplication,
      sol::overload(
          [](const glm::mat4& a, const glm::mat4& b) { return a * b; },
          [](const glm::mat4& m, const glm::vec4& v) { return m * v; }),
      sol::meta_function::to_string,
      [](const glm::mat4& /*unused*/) { return "mat4(...)"; });

  // mat4 static factory methods as a table
  sol::table mat4_meta = uinta["mat4"].get_or_create<sol::table>();
  mat4_meta["identity"] = []() { return glm::mat4(1.0f); };
  mat4_meta["lookAt"] = [](const glm::vec3& eye, const glm::vec3& center,
                           const glm::vec3& up) {
    return glm::lookAt(eye, center, up);
  };
  mat4_meta["perspective"] = [](float fov, float aspect, float near,
                                float far) {
    return glm::perspective(fov, aspect, near, far);
  };
  mat4_meta["ortho"] = [](float left, float right, float bottom, float top,
                          float near, float far) {
    return glm::ortho(left, right, bottom, top, near, far);
  };
  mat4_meta["translate"] = [](const glm::mat4& m, const glm::vec3& v) {
    return glm::translate(m, v);
  };
  mat4_meta["rotate"] = [](const glm::mat4& m, float angle,
                           const glm::vec3& axis) {
    return glm::rotate(m, angle, axis);
  };
  mat4_meta["scale"] = [](const glm::mat4& m, const glm::vec3& v) {
    return glm::scale(m, v);
  };

  // Also allow uinta.mat4() to create identity
  uinta["mat4"] = sol::overload([]() { return glm::mat4(1.0f); },
                                [](float v) { return glm::mat4(v); });

  // Math utilities under uinta.math
  sol::table math = uinta["math"].get_or_create<sol::table>();

  math["clamp"] = [](float value, float min, float max) {
    return std::clamp(value, min, max);
  };

  math["lerp"] = [](float a, float b, float t) { return a + ((b - a) * t); };

  math["smoothstep"] = [](float edge0, float edge1, float x) {
    float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - (2.0f * t));
  };

  math["lerpVec3"] = [](const glm::vec3& a, const glm::vec3& b, float t) {
    return a + (b - a) * t;
  };

  math["clampVec3"] = [](const glm::vec3& v, const glm::vec3& min,
                         const glm::vec3& max) {
    return glm::vec3(std::clamp(v.x, min.x, max.x),
                     std::clamp(v.y, min.y, max.y),
                     std::clamp(v.z, min.z, max.z));
  };
}

}  // namespace uinta
