#include "uinta/lua/input_bindings.h"

#include <absl/log/log.h>

#include <cassert>

#include "uinta/engine/engine.h"
#include "uinta/input/input_system.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <sol/sol.hpp>  // IWYU pragma: keep
#pragma GCC diagnostic pop

namespace uinta {

InputBindings::InputBindings(Engine* engine) noexcept : engine_(engine) {}

void InputBindings::registerBindings(sol::state& lua) {
  registerEnums(lua);
  registerInputFunctions(lua);
}

void InputBindings::onBeforeReload() {
  auto* inputSystem = engine_->service<InputSystem>();
  assert(inputSystem != nullptr &&
         "InputSystem must be registered as a service before using input "
         "bindings");

  // Unsubscribe all Lua handles
  for (const auto& handle : activeHandles_) {
    inputSystem->unsubscribe(handle);
  }

  activeHandles_.clear();
}

void InputBindings::registerEnums(sol::state& lua) {
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();

  // Key enum - expose 10 keys as requested
  sol::table keyTable = uinta["Key"].get_or_create<sol::table>();
  keyTable["Space"] = Key::Space;
  keyTable["Apostrophe"] = Key::Apostrophe;
  keyTable["Comma"] = Key::Comma;
  keyTable["Minus"] = Key::Minus;
  keyTable["Period"] = Key::Period;
  keyTable["Slash"] = Key::Slash;
  keyTable["Num0"] = Key::Num0;
  keyTable["Num1"] = Key::Num1;
  keyTable["Num2"] = Key::Num2;
  keyTable["Num3"] = Key::Num3;
  keyTable["Num4"] = Key::Num4;
  keyTable["Num5"] = Key::Num5;
  keyTable["Num6"] = Key::Num6;
  keyTable["Num7"] = Key::Num7;
  keyTable["Num8"] = Key::Num8;
  keyTable["Num9"] = Key::Num9;
  keyTable["Semicolon"] = Key::Semicolon;
  keyTable["Equal"] = Key::Equal;
  keyTable["A"] = Key::A;
  keyTable["B"] = Key::B;
  keyTable["C"] = Key::C;
  keyTable["D"] = Key::D;
  keyTable["E"] = Key::E;
  keyTable["F"] = Key::F;
  keyTable["G"] = Key::G;
  keyTable["H"] = Key::H;
  keyTable["I"] = Key::I;
  keyTable["J"] = Key::J;
  keyTable["K"] = Key::K;
  keyTable["L"] = Key::L;
  keyTable["M"] = Key::M;
  keyTable["N"] = Key::N;
  keyTable["O"] = Key::O;
  keyTable["P"] = Key::P;
  keyTable["Q"] = Key::Q;
  keyTable["R"] = Key::R;
  keyTable["S"] = Key::S;
  keyTable["T"] = Key::T;
  keyTable["U"] = Key::U;
  keyTable["V"] = Key::V;
  keyTable["W"] = Key::W;
  keyTable["X"] = Key::X;
  keyTable["Y"] = Key::Y;
  keyTable["Z"] = Key::Z;
  keyTable["LeftBracket"] = Key::LeftBracket;
  keyTable["Backslash"] = Key::Backslash;
  keyTable["RightBracket"] = Key::RightBracket;
  keyTable["GraveAccent"] = Key::GraveAccent;
  keyTable["Escape"] = Key::Escape;
  keyTable["Enter"] = Key::Enter;
  keyTable["Tab"] = Key::Tab;
  keyTable["Backspace"] = Key::Backspace;
  keyTable["Insert"] = Key::Insert;
  keyTable["Delete"] = Key::Delete;
  keyTable["Right"] = Key::Right;
  keyTable["Left"] = Key::Left;
  keyTable["Down"] = Key::Down;
  keyTable["Up"] = Key::Up;
  keyTable["PageUp"] = Key::PageUp;
  keyTable["PageDown"] = Key::PageDown;
  keyTable["Home"] = Key::Home;
  keyTable["End"] = Key::End;
  keyTable["CapsLock"] = Key::CapsLock;
  keyTable["ScrollLock"] = Key::ScrollLock;
  keyTable["NumLock"] = Key::NumLock;
  keyTable["PrintScreen"] = Key::PrintScreen;
  keyTable["Pause"] = Key::Pause;
  keyTable["F1"] = Key::F1;
  keyTable["F2"] = Key::F2;
  keyTable["F3"] = Key::F3;
  keyTable["F4"] = Key::F4;
  keyTable["F5"] = Key::F5;
  keyTable["F6"] = Key::F6;
  keyTable["F7"] = Key::F7;
  keyTable["F8"] = Key::F8;
  keyTable["F9"] = Key::F9;
  keyTable["F10"] = Key::F10;
  keyTable["F11"] = Key::F11;
  keyTable["F12"] = Key::F12;
  keyTable["F13"] = Key::F13;
  keyTable["F14"] = Key::F14;
  keyTable["F15"] = Key::F15;
  keyTable["F16"] = Key::F16;
  keyTable["F17"] = Key::F17;
  keyTable["F18"] = Key::F18;
  keyTable["F19"] = Key::F19;
  keyTable["F20"] = Key::F20;
  keyTable["F21"] = Key::F21;
  keyTable["F22"] = Key::F22;
  keyTable["F23"] = Key::F23;
  keyTable["F24"] = Key::F24;
  keyTable["F25"] = Key::F25;
  keyTable["Kp0"] = Key::Kp0;
  keyTable["Kp1"] = Key::Kp1;
  keyTable["Kp2"] = Key::Kp2;
  keyTable["Kp3"] = Key::Kp3;
  keyTable["Kp4"] = Key::Kp4;
  keyTable["Kp5"] = Key::Kp5;
  keyTable["Kp6"] = Key::Kp6;
  keyTable["Kp7"] = Key::Kp7;
  keyTable["Kp8"] = Key::Kp8;
  keyTable["Kp9"] = Key::Kp9;
  keyTable["KpDecimal"] = Key::KpDecimal;
  keyTable["KpDivide"] = Key::KpDivide;
  keyTable["KpMultiply"] = Key::KpMultiply;
  keyTable["KpSubtract"] = Key::KpSubtract;
  keyTable["KpAdd"] = Key::KpAdd;
  keyTable["KpEnter"] = Key::KpEnter;
  keyTable["KpEqual"] = Key::KpEqual;
  keyTable["LeftShift"] = Key::LeftShift;
  keyTable["LeftControl"] = Key::LeftControl;
  keyTable["LeftAlt"] = Key::LeftAlt;
  keyTable["LeftSuper"] = Key::LeftSuper;
  keyTable["RightShift"] = Key::RightShift;
  keyTable["RightControl"] = Key::RightControl;
  keyTable["RightAlt"] = Key::RightAlt;
  keyTable["RightSuper"] = Key::RightSuper;
  keyTable["Menu"] = Key::Menu;

  // Action enum - all 3 actions
  sol::table actionTable = uinta["Action"].get_or_create<sol::table>();
  actionTable["Press"] = Action::Press;
  actionTable["Release"] = Action::Release;
  actionTable["Repeat"] = Action::Repeat;

  // Mod enum - 4 main modifiers
  sol::table modTable = uinta["Mod"].get_or_create<sol::table>();
  modTable["Shift"] = Mod::Shift;
  modTable["Control"] = Mod::Control;
  modTable["Alt"] = Mod::Alt;
  modTable["Super"] = Mod::Super;

  // MouseBtn enum - 3 main buttons
  sol::table mouseBtnTable = uinta["MouseBtn"].get_or_create<sol::table>();
  mouseBtnTable["Left"] = MouseBtn::Left;
  mouseBtnTable["Right"] = MouseBtn::Right;
  mouseBtnTable["Middle"] = MouseBtn::Middle;
}

void InputBindings::registerInputFunctions(sol::state& lua) {
  sol::table uinta = lua["uinta"].get_or_create<sol::table>();
  sol::table input = uinta["input"].get_or_create<sol::table>();

  // onKey: subscribe to keyboard input
  input["onKey"] = [this](u32 token, sol::function callback) {
    auto* inputSystem = engine_->service<InputSystem>();
    assert(inputSystem != nullptr &&
           "InputSystem must be registered as a service before using input "
           "bindings");

    auto handle = inputSystem->subscribeKey(
        token, [callback](Key key, Action action, Mod mods) {
          callback(static_cast<u32>(key), static_cast<u32>(action),
                   static_cast<u32>(mods));
        });

    activeHandles_.push_back(handle);
    return handle;
  };

  // onMouseButton: subscribe to mouse button input
  input["onMouseButton"] = [this](u32 token, sol::function callback) {
    auto* inputSystem = engine_->service<InputSystem>();
    assert(inputSystem != nullptr &&
           "InputSystem must be registered as a service before using input "
           "bindings");

    auto handle = inputSystem->subscribeMouse(
        token, [callback](MouseBtn button, Action action, Mod mods) {
          callback(static_cast<u32>(button), static_cast<u32>(action),
                   static_cast<u32>(mods));
        });

    activeHandles_.push_back(handle);
    return handle;
  };

  // onMouseMove: subscribe to mouse movement
  input["onMouseMove"] = [this](sol::function callback) {
    auto* inputSystem = engine_->service<InputSystem>();
    assert(inputSystem != nullptr &&
           "InputSystem must be registered as a service before using input "
           "bindings");

    auto handle = inputSystem->subscribeMouseMove(
        [callback](f32 x, f32 y, f32 dx, f32 dy) { callback(x, y, dx, dy); });

    activeHandles_.push_back(handle);
    return handle;
  };

  // onMouseScroll: subscribe to mouse scroll
  input["onMouseScroll"] = [this](sol::function callback) {
    auto* inputSystem = engine_->service<InputSystem>();
    assert(inputSystem != nullptr &&
           "InputSystem must be registered as a service before using input "
           "bindings");

    auto handle = inputSystem->subscribeMouseScroll(
        [callback](f32 dx, f32 dy) { callback(dx, dy); });

    activeHandles_.push_back(handle);
    return handle;
  };

  // off: unsubscribe from input events
  input["off"] = [this](SubscriptionHandle handle) {
    auto* inputSystem = engine_->service<InputSystem>();
    assert(inputSystem != nullptr &&
           "InputSystem must be registered as a service before using input "
           "bindings");

    inputSystem->unsubscribe(handle);

    // Remove from our tracking vector
    auto it = std::find_if(
        activeHandles_.begin(), activeHandles_.end(),
        [&handle](const SubscriptionHandle& h) { return h.id == handle.id; });
    if (it != activeHandles_.end()) {
      activeHandles_.erase(it);
    }
  };
}

}  // namespace uinta
