#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include <math.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

const auto background = glm::vec3(216, 204, 192) / glm::vec3(255.0f);
smooth_float ortho_size = smooth_float(5.0, 10.0);
int imgui_level = 3;

struct rayPickingRunner final : glfw_runner {
    debug_camera camera;

    GLuint vao;
    gl_buf vbo;
    gl_buf ebo;

    glm::vec2 cursor_pos = glm::vec2(0.0);

    glm::mat4 m_proj;
    glm::mat4 mvp;

    GLuint shader;
    GLuint u_mvp;

    glm::vec2 ndc_space;
    glm::vec4 clip_space;
    glm::vec4 eye_space;
    glm::vec4 world_space;

    rayPickingRunner() noexcept : glfw_runner("hello ray picking", 1000, 1000) {
        camera.target.z(1.0);
    }

    void doInit() override {
        init_shader();
        init_buffers();
        init_grid();
        init_imgui();
    }

    void init_buffers() {
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

    void init_grid() {
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

    void init_shader() {
        char vert[get_file_size("shader/ray_picking.vert")];
        read_file_raw("shader/ray_picking.vert", vert);

        char frag[get_file_size("shader/ray_picking.frag")];
        read_file_raw("shader/ray_picking.frag", frag);

        const char* sources[] = { vert, frag };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const GLint source_lengths[] = { (GLint) sizeof(vert), (GLint) sizeof(frag) };
        const char* uniforms[] = { "u_mvp" };
        GLuint* locations[] = { &u_mvp };

        shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum), source_lengths,
                                       uniforms, locations, sizeof(locations) / sizeof(GLuint*));
    }

    void init_imgui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui::GetIO().FontGlobalScale = 1.2f;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void doPreTick(const runner_state& state) override {
        // handle input
        cursor_pos = glm::vec2(state.input.cursorx, state.input.cursory);
        updateCursorVectors();

        if (!state.input.isAnyKeyDown()) return;

        float magnitude = 8.0 * state.delta;

        if (state.input.isKeyDown(KEY_W))
            camera.target.smooth_float_y() += magnitude;
        if (state.input.isKeyDown(KEY_A))
            camera.target.smooth_float_x() -= magnitude;
        if (state.input.isKeyDown(KEY_S))
            camera.target.smooth_float_y() -= magnitude;
        if (state.input.isKeyDown(KEY_D))
            camera.target.smooth_float_x() += magnitude;

        // change ortho size
        if (state.input.isKeyDown(KEY_EQUAL)) ortho_size.target = std::max(1.0f, ortho_size.target - magnitude);
        if (state.input.isKeyDown(KEY_MINUS)) ortho_size += magnitude;

        // change imgui scale
        if (state.input.isKeyDown(KEY_I)) state.input.isShiftDown() ? std::max(0, --imgui_level) : std::min(3, ++imgui_level); 

        // reset view
        if (state.input.isKeyDown(KEY_R)) {
            camera.target.smooth_float_x() = 0.0;
            camera.target.smooth_float_y() = 0.0;
            ortho_size.target = 1.0;
        }
    }

    void doTick(const runner_state& state) override {
        camera.tick(state.delta);
        ortho_size.tick(state.delta);
    }

    void doPreRender() override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_proj = glm::ortho((double) -ortho_size.current, (double) ortho_size.current, (double) -ortho_size.current, (double) ortho_size.current, 0.0001, 1000.0);
        camera.update_view_matrix();

        glm::mat4 model(1.0);
        mvp = m_proj * camera.m_view_matrix * model;

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
        if (imgui_level >= 1) ImGui::Text("Device  %+.2f, %+.2f", ndc_space.x, ndc_space.y);
        if (imgui_level >= 2) ImGui::Text("Eye     %+.2f, %+.2f", eye_space.x, eye_space.y);
        if (imgui_level >= 3) ImGui::Text("World   %+.2f, %+.2f", world_space.x, world_space.y);
        ImGui::End();
    }

    void doPostRender() override {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void updateCursorVectors() {
        ndc_space   = glm::vec2(2.0 * cursor_pos.x / view.height - 1.0, -2.0 * cursor_pos.y / view.height + 1.0);
        clip_space  = glm::vec4(ndc_space, -1.0, 1.0);
        eye_space   = glm::inverse(m_proj) * clip_space;
        world_space = glm::inverse(camera.m_view_matrix) * eye_space;
    }

    void doShutdown() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

};

rayPickingRunner runner;

int main(const int argc, const char** argv) {
    runner.init();

    glfwSetKeyCallback(runner.window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS && mods & GLFW_MOD_SHIFT && key == GLFW_KEY_Q) return glfwSetWindowShouldClose(runner.window, true);
        runner.handleKeyInput(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(runner.window, [] (GLFWwindow* window, double xpos, double ypos) {
        runner.handleCursorPositionChanged(xpos, ypos);
    });

    while (!glfwWindowShouldClose(runner.window)) {
        glfwPollEvents();
        runner.tick(glfwGetTime());
        runner.render(background, GL_COLOR_BUFFER_BIT);
    }

    runner.shutdown();
    on_exit([] (int status, void* args) {
        if (runner.window)
            glfwDestroyWindow(runner.window);
        glfwTerminate();
    }, nullptr);

    return 0;
}
