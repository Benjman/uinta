#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uinta/math.hpp>
#include <uinta/camera.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../utils/utils.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

SmoothFloat ortho_size = SmoothFloat(5.0, 10.0);
int imgui_level = 3;

struct RayPickingCameraConfigs : CameraConfig {

};

struct RayPickingRunner final : GlfwRunner {
    SmoothVec3 cam_pos = glm::vec3(0.0);

    GLuint vao;
    gl_buf vbo;
    gl_buf ebo;

    glm::vec2 cursor_pos = glm::vec2(0.0);

    glm::mat4 m_proj;
    glm::mat4 m_view;
    glm::mat4 mvp;

    GLuint shader;
    GLuint u_mvp;

    glm::vec2 ndcSpace;
    glm::vec4 clipSpace;
    glm::vec4 eyeSpace;
    glm::vec4 worldSpace;

    RayPickingRunner() noexcept : GlfwRunner("hello ray picking", 1000, 1000) {
        cam_pos.z(1.0);
    }

    void doInit() override {
        initShader();
        initBuffers();
        initGrid();
        imguiInit();
        setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setBackground(glm::vec3(216, 204, 192) / glm::vec3(255.0f));
    }

    void initBuffers() {
        glGenVertexArrays(1, &vao);

        GLuint ids[2];
        glGenBuffers(2, ids);
        vbo.id = ids[0];
        ebo.id = ids[1];

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }

    void initGrid() {
        const auto rows = 27u,
                   cols = 27u;
        const auto node_size = 0.1f,
                   node_half = node_size * 0.5f,
                   x_start = -std::floor(cols / 2.0f),
                   y_start = -std::floor(rows / 2.0f);
        const auto node_color = glm::vec3(1.0, 0.25, 0.25);
        auto ioff = 0u;

        GLfloat vertices[24 * rows * cols];
        GLuint indices[6 * rows * cols];

        // grid squares
        for (auto y = 0; y < rows; y++) {
            for (auto x = 0; x < cols; x++) {
                auto pos = glm::vec3(x_start + x, y_start + y, 0);

                if (!pos.x || !pos.y)
                    // origin lines are generated below
                    continue;

                auto node_transform = glm::translate(glm::mat4(1.0), pos);

                glm::vec3 vectors[] = {
                    node_transform * glm::vec4(- node_half, + node_half, 0.0f, 1.0f),
                    node_transform * glm::vec4(- node_half, - node_half, 0.0f, 1.0f),
                    node_transform * glm::vec4(+ node_half, - node_half, 0.0f, 1.0f),
                    node_transform * glm::vec4(+ node_half, + node_half, 0.0f, 1.0f),
                };

                GLfloat tmp_vertices[] = {
                 // x             y             z               r                g                b
                    vectors[0].x, vectors[0].y, vectors[0].z,   node_color.r,    node_color.g,    node_color.b,
                    vectors[1].x, vectors[1].y, vectors[1].z,   node_color.r,    node_color.g,    node_color.b,
                    vectors[2].x, vectors[2].y, vectors[2].z,   node_color.r,    node_color.g,    node_color.b,
                    vectors[3].x, vectors[3].y, vectors[3].z,   node_color.r,    node_color.g,    node_color.b,
                };
                memcpy(&vertices[vbo.count], tmp_vertices, sizeof(tmp_vertices));

                GLuint tmp_indices[] {
                    0 + ioff, 1 + ioff, 2 + ioff,
                    2 + ioff, 3 + ioff, 0 + ioff,
                };
                memcpy(&indices[ebo.count], tmp_indices, sizeof(tmp_indices));

                ioff += 4;
                ebo.count += 6;
                vbo.count += 24;
            }
        }

        // x-axis line
        {
            GLfloat tmp_vertices[] = {
            // x         y                   z      r    g    b
               -x_start,  node_half * 0.25f, 0.0,   0.0, 0.5, 0.5,
               -x_start, -node_half * 0.25f, 0.0,   0.0, 0.5, 0.5,
                x_start, -node_half * 0.25f, 0.0,   0.0, 0.5, 0.5,
                x_start,  node_half * 0.25f, 0.0,   0.0, 0.5, 0.5,
            };
            memcpy(&vertices[vbo.count], tmp_vertices, sizeof(tmp_vertices));

            GLuint tmp_indices[] {
                0 + ioff, 1 + ioff, 2 + ioff,
                2 + ioff, 3 + ioff, 0 + ioff,
            };
            memcpy(&indices[ebo.count], tmp_indices, sizeof(tmp_indices));

            ioff += 4;
            ebo.count += 6;
            vbo.count += 24;
        }

        // y-axis line
        {
            GLfloat tmp_vertices[] = {
            // x                    y        z      r    g    b
               -node_half * 0.25f,  y_start, 0.0,   0.5, 0.0, 0.5,
               -node_half * 0.25f, -y_start, 0.0,   0.5, 0.0, 0.5,
                node_half * 0.25f, -y_start, 0.0,   0.5, 0.0, 0.5,
                node_half * 0.25f,  y_start, 0.0,   0.5, 0.0, 0.5,
            };
            memcpy(&vertices[vbo.count], tmp_vertices, sizeof(tmp_vertices));

            GLuint tmp_indices[] {
                0 + ioff, 1 + ioff, 2 + ioff,
                2 + ioff, 3 + ioff, 0 + ioff,
            };
            memcpy(&indices[ebo.count], tmp_indices, sizeof(tmp_indices));

            ioff += 4;
            ebo.count += 6;
            vbo.count += 24;
        }

        // upload buffers
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void initShader() {
        char vert[getFileSize("shader/ray_picking.vert")];
        readFileRaw("shader/ray_picking.vert", vert);

        char frag[getFileSize("shader/ray_picking.frag")];
        readFileRaw("shader/ray_picking.frag", frag);

        const char* sources[] = { vert, frag };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const GLint source_lengths[] = { (GLint) sizeof(vert), (GLint) sizeof(frag) };
        const std::vector<std::string> uniforms({ "u_mvp" });
        const std::vector<GLuint*> locations = { &u_mvp };
        shader = createShaderProgram(sources, stages, sizeof(stages) / sizeof(GLenum), source_lengths,
                                     uniforms, locations);
    }

    void doPreTick(const RunnerState& state) override {
        // handle input
        cursor_pos = glm::vec2(state.input.cursorx, state.input.cursory);
        updateCursorVectors();

        if (!state.input.isAnyKeyDown()) return;

        float magnitude = 8.0 * state.delta;

        if (state.input.isKeyDown(KEY_W))
            cam_pos.smooth_float_y() += magnitude;
        if (state.input.isKeyDown(KEY_A))
            cam_pos.smooth_float_x() -= magnitude;
        if (state.input.isKeyDown(KEY_S))
            cam_pos.smooth_float_y() -= magnitude;
        if (state.input.isKeyDown(KEY_D))
            cam_pos.smooth_float_x() += magnitude;

        // change ortho size
        if (state.input.isKeyDown(KEY_EQUAL)) ortho_size = std::max(1.0f, ortho_size.target - magnitude);
        if (state.input.isKeyDown(KEY_MINUS)) ortho_size += magnitude;

        // change imgui scale
        if (state.input.isKeyDown(KEY_I)) state.input.isShiftDown() ? std::max(0, --imgui_level) : std::min(3, ++imgui_level); 

        // reset view
        if (state.input.isKeyDown(KEY_R)) {
            cam_pos.smooth_float_x() = 0.0;
            cam_pos.smooth_float_y() = 0.0;
            ortho_size = 1.0;
        }
    }

    void doTick(const RunnerState& state) override {
        cam_pos.tick(state.delta);
        ortho_size.tick(state.delta);
    }

    void doPreRender() override {
        imguiPreRender();

        m_proj = glm::ortho((double) -ortho_size.current, (double) ortho_size.current, (double) -ortho_size.current, (double) ortho_size.current, 0.0001, 1000.0);
        updateViewMatrix(m_view, cam_pos, 0, 0);

        glm::mat4 model(1.0);
        mvp = m_proj * m_view * model;

        updateCursorVectors();
    }

    void doRender() override {
        glUseProgram(shader);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &mvp[0][0]);

        glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);

        if (!imgui_level) return;
        ImGui::Begin("Cursor info");
        if (imgui_level >= 1) ImGui::Text("Device  %+.2f, %+.2f", ndcSpace.x, ndcSpace.y);
        if (imgui_level >= 2) ImGui::Text("Eye     %+.2f, %+.2f", eyeSpace.x, eyeSpace.y);
        if (imgui_level >= 3) ImGui::Text("World   %+.2f, %+.2f", worldSpace.x, worldSpace.y);
        ImGui::End();
    }

    void doPostRender() override {
        imguiPostRender();
    }

    void updateCursorVectors() {
        ndcSpace   = glm::vec2(2.0 * cursor_pos.x / display.height - 1.0, -2.0 * cursor_pos.y / display.height + 1.0);
        clipSpace  = glm::vec4(ndcSpace, -1.0, 1.0);
        eyeSpace   = glm::inverse(m_proj) * clipSpace;
        worldSpace = glm::inverse(m_view) * eyeSpace;
    }

    void doShutdown() override {
        imguiShutdown();
    }

};

RayPickingRunner runner;

int main(const int argc, const char** argv) {
    return runner.run();
}
