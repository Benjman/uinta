#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstring>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.hpp>
#include <file.hpp>
#include <mesh.hpp>
#include <model.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

const unsigned int VBUF_SIZE = KILOBYTES(15);
const unsigned int IBUF_SIZE = KILOBYTES(15);

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;

struct camera3dRunner final : glfw_runner {
public:
    camera_controller cam;

    GLuint u_mvp;
    glm::mat4 model = glm::mat4(1.0);

    GLuint shader;
    GLuint vao;
    gl_buf vbo;
    gl_buf ebo;

    camera3dRunner() : glfw_runner("hello camera3d", 1000, 1000) {
        cam.target_y.force(15.0);
        cam.target_z.force(60.0);
        cam.target_pitch.force(-0.0);
    }

    void doInit() override {
        init_shader();

        GLfloat vertices[KILOBYTES(20)];
        GLuint indices[KILOBYTES(20)];
        init_ground(vertices, indices);
        init_buffers(vertices, indices);
    }

    void init_buffers(GLfloat* const vertices, GLuint* const indices) {
        glGenVertexArrays(1, &vao);

        GLuint ids[2];
        glGenBuffers(2, ids);
        vbo.id = ids[0];
        ebo.id = ids[1];

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

        glBufferData(GL_ARRAY_BUFFER, vbo.count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.count * sizeof(GLuint), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
    }

    void init_ground(GLfloat* const vertices, GLuint* const indices) {
        unsigned int local_vcount = 0,
                     local_icount = 0;
            
        mesh_attrib pos_attrib(3, 9, 0),
                    norm_attrib(3, 9, 3),
                    color_attrib(3, 9, 6);

        const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
            {MeshAttribType_Position, pos_attrib},
            {MeshAttribType_Normal, norm_attrib},
            {MeshAttribType_Color, color_attrib},
        };
        loadObj(Model_Cube, vertices, &local_vcount, indices, &local_icount, &attribs);


        glm::vec3 grass(0.0, 1.0, 0.0);
        glm::vec3 dirt = glm::vec3(165, 42, 42) / glm::vec3(255);
        glm::mat4 transform = glm::scale(glm::mat4(1.0), glm::vec3(30, 1, 30));

        local_vcount *= 1.5; // loadObj doesn't load colors, so we adjust for color attrib
        for (int i = 0; i < local_vcount; i += pos_attrib.stride) {
            // transform from cube to something resembling a floor
            glm::vec3 pos = transform * glm::vec4(vertices[i + pos_attrib.offset + 0], vertices[i + pos_attrib.offset + 1], vertices[i + pos_attrib.offset + 2], 1.0);
 
            memcpy(&vertices[i + pos_attrib.offset], &pos[0], 3 * sizeof(GLfloat));

            { // colorize
                glm::vec3 color(0);
                glm::vec3 norm = glm::vec4(vertices[i + norm_attrib.offset + 0], vertices[i + norm_attrib.offset + 1], vertices[i + norm_attrib.offset + 2], 0.0);

                color += dirt  * glm::abs(glm::dot(norm, glm::vec3(1, 0, 1)));          // paint sides
                color += grass * std::max(0.0f, glm::dot(norm, glm::vec3(0, 1, 0)));    // paint top

                memcpy(&vertices[i + color_attrib.offset], &color[0], 3 * sizeof(GLfloat));
            }
        }

        vbo.count += local_vcount;
        ebo.count += local_icount;
    }

    void init_shader() {
        char vshader[get_file_size("shader/camera3d.vert")];
        char fshader[get_file_size("shader/camera3d.frag")];

        read_file_raw("shader/camera3d.vert", vshader);
        read_file_raw("shader/camera3d.frag", fshader);

        const char* sources[] = { vshader, fshader };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const GLint buffer_lengths[] = { (GLint) sizeof(vshader), (GLint) sizeof(fshader) };
        const char* uniforms[] = { "u_mvp" };
        GLuint* locations[] = { &u_mvp };
        shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum), buffer_lengths,
                                       uniforms, locations, sizeof(locations) / sizeof(GLuint*));
    }

    void doTick(const runner_state &state) override {
        // model = glm::rotate(glm::mat4(1.0), state.runtime * 0.25f, glm::vec3(0, 1, 0));

        const float camSpeed = 15.0;
        if (state.input.isKeyDown(GLFW_KEY_W)) cam.target_z -= camSpeed * state.dt;
        if (state.input.isKeyDown(GLFW_KEY_S)) cam.target_z += camSpeed * state.dt;
        if (state.input.isKeyDown(GLFW_KEY_A)) cam.target_x -= camSpeed * state.dt;
        if (state.input.isKeyDown(GLFW_KEY_D)) cam.target_x += camSpeed * state.dt;

        cam.tick(state.dt);
    }

    void doPreRender() override {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 view_mat(1.0);
        cam.view_matrix(&view_mat);
    }

    void doRender() override {
        glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);
    }

};

camera3dRunner runner;

int main(const int argc, const char** argv) {
    runner.init();

    glfwSetKeyCallback(runner.window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        runner.handleKeyInput(key, scancode, action, mods);
    });

    while (!glfwWindowShouldClose(runner.window)) {
        glfwPollEvents();
        do {
            runner.tick(glfwGetTime());
        } while(!runner.shouldRenderFrame());
        runner.render(glm::vec3(0.2f, 0.3f, 0.3f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    runner.shutdown();
    on_exit([] (int status, void* arg) {
        if (runner.window)
            glfwDestroyWindow(runner.window);
        glfwTerminate();
    }, nullptr);

    return 0;
}
