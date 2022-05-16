#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <exception>

#include <glfw.hpp>
#include <stdexcept>
#include <string>

viewport::viewport(const std::string& title, unsigned int width, unsigned int height) noexcept :
    title(std::string(title)), width(width), height(height) {
}

void createGLFWWindow(viewport* const view) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    printf("[INFO] Creating GLFW window \"%s\" (%dx%d)...\n", view->title.c_str(), view->width, view->height);
    view->window = glfwCreateWindow(view->width, view->height, view->title.c_str(), NULL, NULL);
    if (view->window == NULL) {
        glfwTerminate();
        printf("[ERROR] Failed to create GLFW window.\n"); // TODO logging
        throw std::exception();
        return;
    }
    printf("[INFO] Completed creating GLFW window \"%s\" (%dx%d).\n", view->title.c_str(), view->width, view->height);
    glfwMakeContextCurrent(view->window);

    printf("[INFO] Loading GLAD...\n");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Failed to load GLAD.\n"); // TODO logging
        throw std::exception();
    }
    printf("[INFO] Done loading GLAD.\n");
}

const char *getKeyStr(int key) noexcept {
    if (key == GLFW_KEY_SPACE) return "SPACE";
    if (key == GLFW_KEY_APOSTROPHE) return "APOSTROPHE";
    if (key == GLFW_KEY_COMMA) return "COMMA";
    if (key == GLFW_KEY_MINUS) return "MINUS";
    if (key == GLFW_KEY_PERIOD) return "PERIOD";
    if (key == GLFW_KEY_SLASH) return "SLASH";
    if (key == GLFW_KEY_0) return "0";
    if (key == GLFW_KEY_1) return "1";
    if (key == GLFW_KEY_2) return "2";
    if (key == GLFW_KEY_3) return "3";
    if (key == GLFW_KEY_4) return "4";
    if (key == GLFW_KEY_5) return "5";
    if (key == GLFW_KEY_6) return "6";
    if (key == GLFW_KEY_7) return "7";
    if (key == GLFW_KEY_8) return "8";
    if (key == GLFW_KEY_9) return "9";
    if (key == GLFW_KEY_SEMICOLON) return "SEMICOLON";
    if (key == GLFW_KEY_EQUAL) return "EQUAL";
    if (key == GLFW_KEY_A) return "A";
    if (key == GLFW_KEY_B) return "B";
    if (key == GLFW_KEY_C) return "C";
    if (key == GLFW_KEY_D) return "D";
    if (key == GLFW_KEY_E) return "E";
    if (key == GLFW_KEY_F) return "F";
    if (key == GLFW_KEY_G) return "G";
    if (key == GLFW_KEY_H) return "H";
    if (key == GLFW_KEY_I) return "I";
    if (key == GLFW_KEY_J) return "J";
    if (key == GLFW_KEY_K) return "K";
    if (key == GLFW_KEY_L) return "L";
    if (key == GLFW_KEY_M) return "M";
    if (key == GLFW_KEY_N) return "N";
    if (key == GLFW_KEY_O) return "O";
    if (key == GLFW_KEY_P) return "P";
    if (key == GLFW_KEY_Q) return "Q";
    if (key == GLFW_KEY_R) return "R";
    if (key == GLFW_KEY_S) return "S";
    if (key == GLFW_KEY_T) return "T";
    if (key == GLFW_KEY_U) return "U";
    if (key == GLFW_KEY_V) return "V";
    if (key == GLFW_KEY_W) return "W";
    if (key == GLFW_KEY_X) return "X";
    if (key == GLFW_KEY_Y) return "Y";
    if (key == GLFW_KEY_Z) return "Z";
    if (key == GLFW_KEY_LEFT_BRACKET) return "LEFT_BRACKET";
    if (key == GLFW_KEY_BACKSLASH) return "BACKSLASH";
    if (key == GLFW_KEY_RIGHT_BRACKET) return "RIGHT_BRACKET";
    if (key == GLFW_KEY_GRAVE_ACCENT) return "GRAVE_ACCENT";
    if (key == GLFW_KEY_WORLD_1) return "WORLD_1";
    if (key == GLFW_KEY_WORLD_2) return "WORLD_2";
    if (key == GLFW_KEY_ESCAPE) return "ESCAPE";
    if (key == GLFW_KEY_ENTER) return "ENTER";
    if (key == GLFW_KEY_TAB) return "TAB";
    if (key == GLFW_KEY_BACKSPACE) return "BACKSPACE";
    if (key == GLFW_KEY_INSERT) return "INSERT";
    if (key == GLFW_KEY_DELETE) return "DELETE";
    if (key == GLFW_KEY_RIGHT) return "RIGHT";
    if (key == GLFW_KEY_LEFT) return "LEFT";
    if (key == GLFW_KEY_DOWN) return "DOWN";
    if (key == GLFW_KEY_UP) return "UP";
    if (key == GLFW_KEY_PAGE_UP) return "PAGE_UP";
    if (key == GLFW_KEY_PAGE_DOWN) return "PAGE_DOWN";
    if (key == GLFW_KEY_HOME) return "HOME";
    if (key == GLFW_KEY_END) return "END";
    if (key == GLFW_KEY_CAPS_LOCK) return "CAPS_LOCK";
    if (key == GLFW_KEY_SCROLL_LOCK) return "SCROLL_LOCK";
    if (key == GLFW_KEY_NUM_LOCK) return "NUM_LOCK";
    if (key == GLFW_KEY_PRINT_SCREEN) return "PRINT_SCREEN";
    if (key == GLFW_KEY_PAUSE) return "PAUSE";
    if (key == GLFW_KEY_F1) return "F1";
    if (key == GLFW_KEY_F2) return "F2";
    if (key == GLFW_KEY_F3) return "F3";
    if (key == GLFW_KEY_F4) return "F4";
    if (key == GLFW_KEY_F5) return "F5";
    if (key == GLFW_KEY_F6) return "F6";
    if (key == GLFW_KEY_F7) return "F7";
    if (key == GLFW_KEY_F8) return "F8";
    if (key == GLFW_KEY_F9) return "F9";
    if (key == GLFW_KEY_F10) return "F10";
    if (key == GLFW_KEY_F11) return "F11";
    if (key == GLFW_KEY_F12) return "F12";
    if (key == GLFW_KEY_F13) return "F13";
    if (key == GLFW_KEY_F14) return "F14";
    if (key == GLFW_KEY_F15) return "F15";
    if (key == GLFW_KEY_F16) return "F16";
    if (key == GLFW_KEY_F17) return "F17";
    if (key == GLFW_KEY_F18) return "F18";
    if (key == GLFW_KEY_F19) return "F19";
    if (key == GLFW_KEY_F20) return "F20";
    if (key == GLFW_KEY_F21) return "F21";
    if (key == GLFW_KEY_F22) return "F22";
    if (key == GLFW_KEY_F23) return "F23";
    if (key == GLFW_KEY_F24) return "F24";
    if (key == GLFW_KEY_F25) return "F25";
    if (key == GLFW_KEY_KP_0) return "KP_0";
    if (key == GLFW_KEY_KP_1) return "KP_1";
    if (key == GLFW_KEY_KP_2) return "KP_2";
    if (key == GLFW_KEY_KP_3) return "KP_3";
    if (key == GLFW_KEY_KP_4) return "KP_4";
    if (key == GLFW_KEY_KP_5) return "KP_5";
    if (key == GLFW_KEY_KP_6) return "KP_6";
    if (key == GLFW_KEY_KP_7) return "KP_7";
    if (key == GLFW_KEY_KP_8) return "KP_8";
    if (key == GLFW_KEY_KP_9) return "KP_9";
    if (key == GLFW_KEY_KP_DECIMAL) return "KP_DECIMAL";
    if (key == GLFW_KEY_KP_DIVIDE) return "KP_DIVIDE";
    if (key == GLFW_KEY_KP_MULTIPLY) return "KP_MULTIPLY";
    if (key == GLFW_KEY_KP_SUBTRACT) return "KP_SUBTRACT";
    if (key == GLFW_KEY_KP_ADD) return "KP_ADD";
    if (key == GLFW_KEY_KP_ENTER) return "KP_ENTER";
    if (key == GLFW_KEY_KP_EQUAL) return "KP_EQUAL";
    if (key == GLFW_KEY_LEFT_SHIFT) return "LEFT_SHIFT";
    if (key == GLFW_KEY_LEFT_CONTROL) return "LEFT_CONTROL";
    if (key == GLFW_KEY_LEFT_ALT) return "LEFT_ALT";
    if (key == GLFW_KEY_LEFT_SUPER) return "LEFT_SUPER";
    if (key == GLFW_KEY_RIGHT_SHIFT) return "RIGHT_SHIFT";
    if (key == GLFW_KEY_RIGHT_CONTROL) return "RIGHT_CONTROL";
    if (key == GLFW_KEY_RIGHT_ALT) return "RIGHT_ALT";
    if (key == GLFW_KEY_RIGHT_SUPER) return "RIGHT_SUPER";
    if (key == GLFW_KEY_MENU) return "MENU";
    return "KEY_UNKNOWN";
}

const char *getActionStr(int action) noexcept {
    if (action == GLFW_PRESS) return "press";
    if (action == GLFW_RELEASE) return "release";
    if (action == GLFW_REPEAT) return "repeat";
    return "ACTION UNKNOWN";
}

const char *getModsStr(int mods) noexcept {
    // TODO implement mods string
    if (mods == 0) return "";
    return "NOT IMPLEMENTED";
}
