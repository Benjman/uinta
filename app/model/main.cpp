#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <unordered_map>

#include <file.hpp>
#include <glfw.hpp>
#include <macros.hpp>
#include <model.hpp>
#include <mesh.hpp>

struct runner {
    viewport view;
    unsigned int frame = 0;

    void init() {
        view.width = 1000;
        view.height = 1000;
        view.title = "hello models";
        createGLFWWindow(view);

        const char* obj[getObjFileSize(Model_Tri)];
        float vbuf[KILOBYTES(3)];
        unsigned int ibuf[KILOBYTES(2)];

        const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
            {MeshAttribType_Position, mesh_attrib(3, 6, 0)},
            {MeshAttribType_Normal, mesh_attrib(3, 6, 3)},
        };

        loadObj(Model_Tri, vbuf, ibuf, &attribs, attribs.size());
    }

    void render() {
    }

};

GLFWwindow* window = nullptr;
void on_exit_handler(int status, void *arg) {
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

int main(const int argc, const char **argv) {
    runner runner;
    runner.init();
    window = runner.view.window;

    while (!glfwWindowShouldClose(runner.view.window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        runner.render();

        glfwSwapBuffers(window);
    }

    on_exit(on_exit_handler, nullptr);

    return 0;
}

