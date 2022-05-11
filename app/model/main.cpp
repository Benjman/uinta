#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

#include <file.hpp>
#include <glfw.hpp>
#include <macros.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <shader.hpp>

struct runner final {
    viewport view;
    unsigned int icount = 0;
    GLuint u_model;

    void init() {
        // initialize viewport
        view.width = 1000;
        view.height = 1000;
        view.title = "hello models";
        createGLFWWindow(view);


        // obj loading
        const Models model = Model_Suzanne;
        float vbuf[getObjFileSize(model)];
        unsigned int ibuf[getObjFileSize(model)];

        const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
            {MeshAttribType_Position, mesh_attrib(3, 6, 0)},
            {MeshAttribType_Normal, mesh_attrib(3, 6, 3)},
        };

        loadObj(Model_Suzanne, vbuf, ibuf, &icount, &attribs, attribs.size());


        // upload model data to the gpu
        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vbuf), vbuf, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibuf), ibuf, GL_STATIC_DRAW);


        // create shaders
        const char *vshader =
            "#version 330 core\n"
            "layout (location = 0) in vec3 in_pos;"
            "layout (location = 1) in vec3 in_norm;"
            "uniform mat4 u_model = mat4(1.0);"
            "out vec3 pass_norm;"
            "void main() {"
            "  pass_norm = normalize(in_norm);"
            "  gl_Position = u_model * vec4(in_pos, 1.0);"
            "}\0";

        const char *fshader =
            "#version 330 core\n"
            "in vec3 pass_norm;"
            "const vec3 obj_color = vec3(0.3, 0.1, 0.1);"
            "void main() {"
            "  gl_FragColor = vec4(obj_color, 1.0);"
            "}\0";

        const char* sources[] = { vshader, fshader };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const char* uniforms[] = { "u_model" };
        GLuint shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum),
                                       uniforms, &u_model, 1);
    }

    void render() {
        glm::mat4 mat = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, &mat[0][0]);
        glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
    }

};

runner runner;

void on_exit_handler(int status, void *arg) {
    if (runner.view.window)
        glfwDestroyWindow(runner.view.window);
    glfwTerminate();
}

int main(const int argc, const char **argv) {
    runner.init();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(runner.view.window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        runner.render();

        glfwSwapBuffers(runner.view.window);
    }

    on_exit(on_exit_handler, nullptr);
    return 0;
}

