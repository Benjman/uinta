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

struct runner {
    viewport view;
    unsigned int icount = 0;
    GLuint u_mvp;

    void init() {
        view.width = 1000;
        view.height = 1000;
        view.title = "hello models";
        createGLFWWindow(view);

        const char* obj[getObjFileSize(Model_Suzanne)];
        float vbuf[KILOBYTES(80)];

        unsigned int ibuf[KILOBYTES(40)];

        const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
            {MeshAttribType_Position, mesh_attrib(3, 6, 0)},
            {MeshAttribType_Normal, mesh_attrib(3, 6, 3)},
        };

        loadObj(Model_Suzanne, vbuf, ibuf, &icount, &attribs, attribs.size());




        // buffers
        GLuint vao, vbo, ebo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vbuf), vbuf, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ibuf), ibuf, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);




        // shaders
        const char *vshader =
            "#version 330 core\n"
            "layout (location = 0) in vec3 in_pos;"
            "layout (location = 1) in vec3 in_norm;"
            "uniform mat4 u_mvp = mat4(1.0);"
            "out vec3 pass_norm;"
            "void main() {"
            "  pass_norm = normalize(in_norm);"
            "  gl_Position = u_mvp * vec4(in_pos, 1.0);"
            "}\0";

        const char *fshader =
            "#version 330 core\n"
            "in vec3 pass_norm;"
            "vec3 direction = normalize(-vec3(1.0, 0.0, 1.0));"
            "vec3 color = vec3(1.0);"
            "float ambient_strength = 0.2;"
            "const vec3 obj_color = vec3(1.0, 0.5, 0.5);"
            "void main() {"
            "  vec3 norms = normalize(pass_norm);"
            "  float diff = max(dot(pass_norm, direction), 0.0);"
            "  gl_FragColor = vec4(vec3(ambient_strength * obj_color), 1.0);"
            "}\0";

        const char* sources[] = { vshader, fshader };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const char* uniforms[] = { "u_mvp" };
        GLuint shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum),
                                       uniforms, &u_mvp, 1);
        glUseProgram(shader);
    }

    void render() {
        glm::mat4 mat = glm::rotate(glm::mat4(1.0), (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mat[0][0]);
        glDrawElements(GL_TRIANGLES, icount, GL_UNSIGNED_INT, 0);
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

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(runner.view.window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        runner.render();

        glfwSwapBuffers(window);
    }

    on_exit(on_exit_handler, nullptr);

    return 0;
}

