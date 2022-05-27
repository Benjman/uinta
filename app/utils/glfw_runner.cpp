#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glfw_runner.hpp"

#include <input.hpp>

input_key_t KEY_SPACE               = GLFW_KEY_SPACE;
input_key_t KEY_APOSTROPHE          = GLFW_KEY_APOSTROPHE;
input_key_t KEY_COMMA               = GLFW_KEY_COMMA;
input_key_t KEY_MINUS               = GLFW_KEY_MINUS;
input_key_t KEY_PERIOD              = GLFW_KEY_PERIOD;
input_key_t KEY_SLASH               = GLFW_KEY_SLASH;
input_key_t KEY_0                   = GLFW_KEY_0;
input_key_t KEY_1                   = GLFW_KEY_1;
input_key_t KEY_2                   = GLFW_KEY_2;
input_key_t KEY_3                   = GLFW_KEY_3;
input_key_t KEY_4                   = GLFW_KEY_4;
input_key_t KEY_5                   = GLFW_KEY_5;
input_key_t KEY_6                   = GLFW_KEY_6;
input_key_t KEY_7                   = GLFW_KEY_7;
input_key_t KEY_8                   = GLFW_KEY_8;
input_key_t KEY_9                   = GLFW_KEY_9;
input_key_t KEY_SEMICOLON           = GLFW_KEY_SEMICOLON;
input_key_t KEY_EQUAL               = GLFW_KEY_EQUAL;
input_key_t KEY_A                   = GLFW_KEY_A;
input_key_t KEY_B                   = GLFW_KEY_B;
input_key_t KEY_C                   = GLFW_KEY_C;
input_key_t KEY_D                   = GLFW_KEY_D;
input_key_t KEY_E                   = GLFW_KEY_E;
input_key_t KEY_F                   = GLFW_KEY_F;
input_key_t KEY_G                   = GLFW_KEY_G;
input_key_t KEY_H                   = GLFW_KEY_H;
input_key_t KEY_I                   = GLFW_KEY_I;
input_key_t KEY_J                   = GLFW_KEY_J;
input_key_t KEY_K                   = GLFW_KEY_K;
input_key_t KEY_L                   = GLFW_KEY_L;
input_key_t KEY_M                   = GLFW_KEY_M;
input_key_t KEY_N                   = GLFW_KEY_N;
input_key_t KEY_O                   = GLFW_KEY_O;
input_key_t KEY_P                   = GLFW_KEY_P;
input_key_t KEY_Q                   = GLFW_KEY_Q;
input_key_t KEY_R                   = GLFW_KEY_R;
input_key_t KEY_S                   = GLFW_KEY_S;
input_key_t KEY_T                   = GLFW_KEY_T;
input_key_t KEY_U                   = GLFW_KEY_U;
input_key_t KEY_V                   = GLFW_KEY_V;
input_key_t KEY_W                   = GLFW_KEY_W;
input_key_t KEY_X                   = GLFW_KEY_X;
input_key_t KEY_Y                   = GLFW_KEY_Y;
input_key_t KEY_Z                   = GLFW_KEY_Z;
input_key_t KEY_LEFT_BRACKET        = GLFW_KEY_LEFT_BRACKET;
input_key_t KEY_BACKSLASH           = GLFW_KEY_BACKSLASH;
input_key_t KEY_RIGHT_BRACKET       = GLFW_KEY_RIGHT_BRACKET;
input_key_t KEY_GRAVE_ACCENT        = GLFW_KEY_GRAVE_ACCENT;
input_key_t KEY_WORLD_1             = GLFW_KEY_WORLD_1;
input_key_t KEY_WORLD_2             = GLFW_KEY_WORLD_2;
input_key_t KEY_ESCAPE              = GLFW_KEY_ESCAPE;
input_key_t KEY_ENTER               = GLFW_KEY_ENTER;
input_key_t KEY_TAB                 = GLFW_KEY_TAB;
input_key_t KEY_BACKSPACE           = GLFW_KEY_BACKSPACE;
input_key_t KEY_INSERT              = GLFW_KEY_INSERT;
input_key_t KEY_DELETE              = GLFW_KEY_DELETE;
input_key_t KEY_RIGHT               = GLFW_KEY_RIGHT;
input_key_t KEY_LEFT                = GLFW_KEY_LEFT;
input_key_t KEY_DOWN                = GLFW_KEY_DOWN;
input_key_t KEY_UP                  = GLFW_KEY_UP;
input_key_t KEY_PAGE_UP             = GLFW_KEY_PAGE_UP;
input_key_t KEY_PAGE_DOWN           = GLFW_KEY_PAGE_DOWN;
input_key_t KEY_HOME                = GLFW_KEY_HOME;
input_key_t KEY_END                 = GLFW_KEY_END;
input_key_t KEY_CAPS_LOCK           = GLFW_KEY_CAPS_LOCK;
input_key_t KEY_SCROLL_LOCK         = GLFW_KEY_SCROLL_LOCK;
input_key_t KEY_NUM_LOCK            = GLFW_KEY_NUM_LOCK;
input_key_t KEY_PRINT_SCREEN        = GLFW_KEY_PRINT_SCREEN;
input_key_t KEY_PAUSE               = GLFW_KEY_PAUSE;
input_key_t KEY_F1                  = GLFW_KEY_F1;
input_key_t KEY_F2                  = GLFW_KEY_F2;
input_key_t KEY_F3                  = GLFW_KEY_F3;
input_key_t KEY_F4                  = GLFW_KEY_F4;
input_key_t KEY_F5                  = GLFW_KEY_F5;
input_key_t KEY_F6                  = GLFW_KEY_F6;
input_key_t KEY_F7                  = GLFW_KEY_F7;
input_key_t KEY_F8                  = GLFW_KEY_F8;
input_key_t KEY_F9                  = GLFW_KEY_F9;
input_key_t KEY_F10                 = GLFW_KEY_F10;
input_key_t KEY_F11                 = GLFW_KEY_F11;
input_key_t KEY_F12                 = GLFW_KEY_F12;
input_key_t KEY_F13                 = GLFW_KEY_F13;
input_key_t KEY_F14                 = GLFW_KEY_F14;
input_key_t KEY_F15                 = GLFW_KEY_F15;
input_key_t KEY_F16                 = GLFW_KEY_F16;
input_key_t KEY_F17                 = GLFW_KEY_F17;
input_key_t KEY_F18                 = GLFW_KEY_F18;
input_key_t KEY_F19                 = GLFW_KEY_F19;
input_key_t KEY_F20                 = GLFW_KEY_F20;
input_key_t KEY_F21                 = GLFW_KEY_F21;
input_key_t KEY_F22                 = GLFW_KEY_F22;
input_key_t KEY_F23                 = GLFW_KEY_F23;
input_key_t KEY_F24                 = GLFW_KEY_F24;
input_key_t KEY_F25                 = GLFW_KEY_F25;
input_key_t KEY_KP_0                = GLFW_KEY_KP_0;
input_key_t KEY_KP_1                = GLFW_KEY_KP_1;
input_key_t KEY_KP_2                = GLFW_KEY_KP_2;
input_key_t KEY_KP_3                = GLFW_KEY_KP_3;
input_key_t KEY_KP_4                = GLFW_KEY_KP_4;
input_key_t KEY_KP_5                = GLFW_KEY_KP_5;
input_key_t KEY_KP_6                = GLFW_KEY_KP_6;
input_key_t KEY_KP_7                = GLFW_KEY_KP_7;
input_key_t KEY_KP_8                = GLFW_KEY_KP_8;
input_key_t KEY_KP_9                = GLFW_KEY_KP_9;
input_key_t KEY_KP_DECIMAL          = GLFW_KEY_KP_DECIMAL;
input_key_t KEY_KP_DIVIDE           = GLFW_KEY_KP_DIVIDE;
input_key_t KEY_KP_MULTIPLY         = GLFW_KEY_KP_MULTIPLY;
input_key_t KEY_KP_SUBTRACT         = GLFW_KEY_KP_SUBTRACT;
input_key_t KEY_KP_ADD              = GLFW_KEY_KP_ADD;
input_key_t KEY_KP_ENTER            = GLFW_KEY_KP_ENTER;
input_key_t KEY_KP_EQUAL            = GLFW_KEY_KP_EQUAL;
input_key_t KEY_LEFT_SHIFT          = GLFW_KEY_LEFT_SHIFT;
input_key_t KEY_LEFT_CONTROL        = GLFW_KEY_LEFT_CONTROL;
input_key_t KEY_LEFT_ALT            = GLFW_KEY_LEFT_ALT;
input_key_t KEY_LEFT_SUPER          = GLFW_KEY_LEFT_SUPER;
input_key_t KEY_RIGHT_SHIFT         = GLFW_KEY_RIGHT_SHIFT;
input_key_t KEY_RIGHT_CONTROL       = GLFW_KEY_RIGHT_CONTROL;
input_key_t KEY_RIGHT_ALT           = GLFW_KEY_RIGHT_ALT;
input_key_t KEY_RIGHT_SUPER         = GLFW_KEY_RIGHT_SUPER;
input_key_t KEY_MENU                = GLFW_KEY_MENU;
input_key_t ACTION_PRESS            = GLFW_PRESS;
input_key_t ACTION_RELEASE          = GLFW_RELEASE;
input_key_t ACTION_REPEAT           = GLFW_REPEAT;
input_key_t MOD_SHIFT               = GLFW_MOD_SHIFT;
input_key_t MOD_CONTROL             = GLFW_MOD_CONTROL;
input_key_t MOD_ALT                 = GLFW_MOD_ALT;
input_key_t MOD_SUPER               = GLFW_MOD_SUPER;
input_key_t MOD_CAPS_LOCK           = GLFW_MOD_CAPS_LOCK;
input_key_t MOD_NUM_LOCK            = GLFW_MOD_NUM_LOCK;
mouse_button_t MOUSE_BUTTON_1       = GLFW_MOUSE_BUTTON_1;
mouse_button_t MOUSE_BUTTON_2       = GLFW_MOUSE_BUTTON_2;
mouse_button_t MOUSE_BUTTON_3       = GLFW_MOUSE_BUTTON_3;
mouse_button_t MOUSE_BUTTON_4       = GLFW_MOUSE_BUTTON_4;
mouse_button_t MOUSE_BUTTON_5       = GLFW_MOUSE_BUTTON_5;
mouse_button_t MOUSE_BUTTON_6       = GLFW_MOUSE_BUTTON_6;
mouse_button_t MOUSE_BUTTON_7       = GLFW_MOUSE_BUTTON_7;
mouse_button_t MOUSE_BUTTON_8       = GLFW_MOUSE_BUTTON_8;
mouse_button_t MOUSE_BUTTON_LAST    = GLFW_MOUSE_BUTTON_LAST;
mouse_button_t MOUSE_BUTTON_LEFT    = GLFW_MOUSE_BUTTON_LEFT;
mouse_button_t MOUSE_BUTTON_RIGHT   = GLFW_MOUSE_BUTTON_RIGHT;
mouse_button_t MOUSE_BUTTON_MIDDLE  = GLFW_MOUSE_BUTTON_MIDDLE;

glfw_runner::~glfw_runner() {
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

void glfw_runner::internal_init() {
    createGLFWWindow(*this);
    register_callbacks();
}

void glfw_runner::internal_shutdown() {
}

void glfw_runner::swap_buffers() {
    glfwSwapBuffers(window);
}

void glfw_runner::register_callbacks() {
    glfwSetKeyCallback(window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        glfw_runner* runner = (glfw_runner*) glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q)
            return glfwSetWindowShouldClose(runner->window, true);
        runner->handleKeyInput(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(window, [] (GLFWwindow* window, double xpos, double ypos) {
        glfw_runner* runner = (glfw_runner*) glfwGetWindowUserPointer(window);
        runner->handleCursorPositionChanged(xpos, ypos);
    });

    glfwSetMouseButtonCallback(window, [] (GLFWwindow* window, int button, int action, int mods) {
        glfw_runner* runner = (glfw_runner*) glfwGetWindowUserPointer(window);
        runner->handleMouseButtonInput(button, action, mods);
    });
}

void createGLFWWindow(glfw_runner& runner) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    printf("[INFO] Creating GLFW window \"%s\" (%dx%d)...\n", runner.view.title.c_str(), runner.view.width, runner.view.height);
    runner.window = glfwCreateWindow(runner.view.width, runner.view.height, runner.view.title.c_str(), NULL, NULL);
    if (runner.window == NULL) {
        glfwTerminate();
        printf("[ERROR] Failed to create GLFW window.\n"); // TODO logging
        throw std::exception();
        return;
    }

    glfwSetWindowUserPointer(runner.window, &runner);

    printf("[INFO] Completed creating GLFW window \"%s\" (%dx%d).\n", runner.view.title.c_str(), runner.view.width, runner.view.height);
    glfwMakeContextCurrent(runner.window);

    printf("[INFO] Loading GLAD...\n");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Failed to load GLAD.\n"); // TODO logging
        throw std::exception();
    }
    printf("[INFO] Done loading GLAD.\n");
}

double glfw_runner::getRuntime() {
    return glfwGetTime();
}

void glfw_runner::pollInput() {
    glfwPollEvents();
}

bool glfw_runner::shouldExit() {
    return glfwWindowShouldClose(window);
}

