#include "imgui.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <file.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <math.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct camera3dRunner final : glfw_runner {
public:
    Camera camera;

    GLuint u_mvp;
    glm::mat4 model = glm::mat4(1.0);

    GLuint shader;
    GLuint vao;
    gl_buf vbo;
    gl_buf ebo;

    camera3dRunner() : glfw_runner("hello camera3d", 1000, 1000) {
    }

    void doInit() override {
        init_shader();
        GLfloat vertices[KILOBYTES(20)];
        GLuint indices[KILOBYTES(20)];
        init_ground(vertices, indices);
        init_buffers(vertices, indices);
        imguiInit();

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

        const glm::vec3 top(0.051, 0.933, 0.996);
        const glm::vec3 sides = glm::vec3(0.025, 0.465, 0.465);
        const glm::mat4 transform = glm::scale(glm::mat4(1.0), glm::vec3(15, 3, 15));

        local_vcount *= 1.5; // loadObj doesn't load colors, so we adjust for color attrib
        for (int i = 0; i < local_vcount; i += pos_attrib.stride) {
            {
                // transform to floor
                auto pos = transform * glm::vec4(vertices[i + pos_attrib.offset + 0], vertices[i + pos_attrib.offset + 1] - 1, vertices[i + pos_attrib.offset + 2], 1.0);
                memcpy(&vertices[i + pos_attrib.offset], &pos[0], 3 * sizeof(GLfloat));
            }

            {
                // colorize
                glm::vec3 color(0);
                glm::vec3 norm = glm::vec3(vertices[i + norm_attrib.offset + 0], vertices[i + norm_attrib.offset + 1], vertices[i + norm_attrib.offset + 2]);

                color += sides  * glm::abs(glm::dot(norm, glm::vec3(1, 0, 1)));          // paint sides
                color += top * std::max(0.0f, glm::dot(norm, glm::vec3(0, 1, 0)));    // paint top

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

    void doTick(const RunnerState &state) override {
        camera.tick(state);
    }

    void doPreRender() override {
        imguiPreRender();

        glUseProgram(shader);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

        updateViewMatrix(camera.view, camera.position, camera.pitch, camera.yaw);
        glm::mat4 proj_mat = glm::perspective(glm::radians(camera.config.fov), (float) display.width / (float) display.height, camera.config.nearPlane, camera.config.farPlane);
        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &(proj_mat * camera.view * model)[0][0]);
    }

    void doRender() override {
        glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);

        ImGui::Begin("Camera");
        ImGui::Text("Translation:   wasd or right-mouse");
        ImGui::Text("Rotation:      cv or middle-mouse");
        ImGui::Text("Distance:      y-scroll");
        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::BeginChild("Camera");
        ImGui::Text("Position     %+.2f %+.2f %+.2f", camera.position.x, camera.position.y, camera.position.z);
        ImGui::Text("Target       %+.2f %+.2f %+.2f", camera.target.x(), camera.target.y(), camera.target.z());
        ImGui::Text("Pitch        %+.2f", camera.pitch);
        ImGui::Text("Yaw          %+.2f", camera.yaw);
        ImGui::EndChild();
        ImGui::End();

    }

    void doPostRender() override {
        imguiPostRender();
    }

    void doShutdown() override {
        imguiShutdown();
    }

};

camera3dRunner runner;

int main(const int argc, const char** argv) {
    return runner.run();
}
