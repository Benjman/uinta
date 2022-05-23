#ifndef UINTA_INPUT_HPP
#define UINTA_INPUT_HPP

#include <set>
#include <glfw.hpp> // TODO glfw references need to be removed to make android compatible
#include "imgui.h"

using input_key_t = short;

struct input final {
    float cursorx;
    float cursordx;

    float cursory;
    float cursordy;

    int flags = 0;
    std::set<input_key_t> keys_down;
    std::set<input_key_t> keys_pressed;
    std::set<input_key_t> keys_released;
    std::set<input_key_t> keys_repeated;

    void reset() {
        cursordx = 0;
        cursordy = 0;
        keys_pressed.clear();
        keys_released.clear();
        keys_repeated.clear();
    }

    bool isAltPressed() const {
        return flags & GLFW_MOD_ALT;
    }

    bool isCtrlPressed() const {
        return flags & GLFW_MOD_CONTROL;
    }

    bool isShiftPressed() const {
        return flags & GLFW_MOD_SHIFT;
    }

    bool isAnyKeyDown() const {
        return keys_down.size();
    }

    bool isKeyDown(input_key_t key) const {
        for (auto value : keys_down)
            if (value == key)
                return true;
        return false;
    }

    bool isKeyPressed(input_key_t key) const {
        for (auto value : keys_pressed)
            if (value == key)
                return true;
        return false;
    }

    bool isKeyReleased(input_key_t key) const {
        for (auto value : keys_released)
            if (value == key)
                return true;
        return false;
    }

    bool isKeyRepeated(input_key_t key) const {
        for (auto value : keys_repeated)
            if (value == key)
                return true;
        return false;
    }

    void keyPressed(const input_key_t key, const int flags) {
        this->flags = flags;
        keys_pressed.insert(key);
        keys_down.insert(key);
    }

    void keyReleased(const input_key_t key, const int flags) {
        this->flags = flags;
        keys_released.insert(key);
        keys_down.erase(key);
    }

    void keyRepeated(const input_key_t key, const int flags) {
        this->flags = flags;
        keys_repeated.insert(key);
    }

};

#endif // UINTA_INPUT_HPP
