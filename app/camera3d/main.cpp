#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <camera.hpp>
#include <file.hpp>
#include <mesh.hpp>
#include <model.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

struct camera3dRunner final : glfw_runner {
public:
    target_cam camera;

    GLuint u_mvp;
    glm::mat4 model = glm::mat4(1.0);

    GLuint shader;
    GLuint vao;
    gl_buf vbo;
    gl_buf ebo;

    camera3dRunner() : glfw_runner("hello camera3d", 1000, 1000) {
        camera.target.y(15.0);
        camera.target.z(60.0);
        camera.pitch(0.0);
    }

    void doInit() override {
        init_shader();
        GLfloat vertices[KILOBYTES(20)];
        GLuint indices[KILOBYTES(20)];
        init_ground(vertices, indices);
        init_buffers(vertices, indices);

        glEnable(GL_DEPTH_TEST);
        setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
            
        const mesh_attrib pos_attrib(9, 0),
                          norm_attrib(9, 3),
                          color_attrib(9, 6);

        const std::unordered_map<MeshAttribType, mesh_attrib> attribs = {
            {MeshAttribType_Position, pos_attrib},
            {MeshAttribType_Normal, norm_attrib},
            {MeshAttribType_Color, color_attrib},
        };
            
        loadObj(Model_Cube, vertices, &local_vcount, indices, &local_icount, &attribs);

        const glm::vec3 grass(0.0, 1.0, 0.0);
        const glm::vec3 dirt = glm::vec3(165, 42, 42) / glm::vec3(255);
        const glm::mat4 transform = glm::scale(glm::mat4(1.0), glm::vec3(30, 1, 30));

        local_vcount *= 1.5; // loadObj doesn't load colors, so we adjust for color attrib
        for (int i = 0; i < local_vcount; i += pos_attrib.stride) {
            {
                // transform to floor
                auto pos = transform * glm::vec4(vertices[i + pos_attrib.offset + 0], vertices[i + pos_attrib.offset + 1], vertices[i + pos_attrib.offset + 2], 1.0);
                memcpy(&vertices[i + pos_attrib.offset], &pos[0], 3 * sizeof(GLfloat));
            }

            {
                // colorize
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

    void doPreTick(const runner_state &state) override {
        // cursor input

        // keyboard input
        if (state.input.isAnyKeyDown()) {
            float speed = 50.0f * state.delta;
            if (state.input.isKeyDown(KEY_W))
                camera.target.smooth_float_z() -= speed;
            if (state.input.isKeyDown(KEY_S))
                camera.target.smooth_float_z() += speed;
            if (state.input.isKeyDown(KEY_A))
                camera.target.smooth_float_x() -= speed;
            if (state.input.isKeyDown(KEY_D))
                camera.target.smooth_float_x() += speed;
        }
    }

    void doTick(const runner_state &state) override {
        // model = glm::rotate(glm::mat4(1.0), state.runtime * 0.25f, glm::vec3(0, 1, 0));
        camera.tick(state.delta);
    }

    void doPreRender() override {
        glUseProgram(shader);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

        camera.update_view_matrix();
        glm::mat4 proj_mat = glm::perspective(glm::radians(45.0), (double) view.width / (double) view.height, 0.01, 500.0);
        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &(proj_mat * camera.m_view_matrix * model)[0][0]);
    }

    void doRender() override {
        glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);
    }

};

camera3dRunner runner;

int main(const int argc, const char** argv) {
    return runner.run();
}
