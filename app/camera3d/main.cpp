#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <camera.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../app_utils.hpp"

const unsigned int VBUF_SIZE = KILOBYTES(15);
const unsigned int IBUF_SIZE = KILOBYTES(15);

const unsigned int WINDOW_WIDTH = 1000;
const unsigned int WINDOW_HEIGHT = 1000;

struct camera3dRunner final : glfw_runner {
public:
    camera cam;

    GLuint u_mvp;

    float runtime = 0.0;
    GLuint shader;
    GLuint vao;
    gl_buf vbo;

    camera3dRunner() : glfw_runner("hello camera3d", 1000, 1000) {
        cam.pos.y = 5.0;
        cam.pos.z = 7.0;
        cam.pitch(30.0);
    }

    void doInit() override {
        init_shader();
        init_buffers();
        init_mesh();
    }

    void init_buffers() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo.id);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    }

    void init_mesh() {
        GLfloat vertices[] = {
            // positions        // colors
            -1.0, -1.0, -1.0,   1.0, 0.0, 0.0,
             1.0, -1.0, -1.0,   1.0, 0.0, 0.0,
             1.0,  1.0, -1.0,   1.0, 0.0, 0.0,
             1.0,  1.0, -1.0,   1.0, 0.0, 0.0,
            -1.0,  1.0, -1.0,   1.0, 0.0, 0.0,
            -1.0, -1.0, -1.0,   1.0, 0.0, 0.0,

            -1.0, -1.0,  1.0,   0.0, 1.0, 0.0,
             1.0, -1.0,  1.0,   0.0, 1.0, 0.0,
             1.0,  1.0,  1.0,   0.0, 1.0, 0.0,
             1.0,  1.0,  1.0,   0.0, 1.0, 0.0,
            -1.0,  1.0,  1.0,   0.0, 1.0, 0.0,
            -1.0, -1.0,  1.0,   0.0, 1.0, 0.0,

            -1.0,  1.0,  1.0,   0.0, 0.0, 1.0,
            -1.0,  1.0, -1.0,   0.0, 0.0, 1.0,
            -1.0, -1.0, -1.0,   0.0, 0.0, 1.0,
            -1.0, -1.0, -1.0,   0.0, 0.0, 1.0,
            -1.0, -1.0,  1.0,   0.0, 0.0, 1.0,
            -1.0,  1.0,  1.0,   0.0, 0.0, 1.0,

             1.0,  1.0,  1.0,   1.0, 1.0, 0.0,
             1.0,  1.0, -1.0,   1.0, 1.0, 0.0,
             1.0, -1.0, -1.0,   1.0, 1.0, 0.0,
             1.0, -1.0, -1.0,   1.0, 1.0, 0.0,
             1.0, -1.0,  1.0,   1.0, 1.0, 0.0,
             1.0,  1.0,  1.0,   1.0, 1.0, 0.0,

            -1.0, -1.0, -1.0,   1.0, 0.0, 1.0,
             1.0, -1.0, -1.0,   1.0, 0.0, 1.0,
             1.0, -1.0,  1.0,   1.0, 0.0, 1.0,
             1.0, -1.0,  1.0,   1.0, 0.0, 1.0,
            -1.0, -1.0,  1.0,   1.0, 0.0, 1.0,
            -1.0, -1.0, -1.0,   1.0, 0.0, 1.0,

            -1.0,  1.0, -1.0,   0.0, 1.0, 1.0,
             1.0,  1.0, -1.0,   0.0, 1.0, 1.0,
             1.0,  1.0,  1.0,   0.0, 1.0, 1.0,
             1.0,  1.0,  1.0,   0.0, 1.0, 1.0,
            -1.0,  1.0,  1.0,   0.0, 1.0, 1.0,
            -1.0,  1.0, -1.0,   0.0, 1.0, 1.0
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        vbo.count = 36;
    }

    void init_shader() {
        const char *vshader =
            "#version 330 core\n"
            "layout (location = 0) in vec3 in_pos;"
            "layout (location = 1) in vec3 in_color;"
            "uniform mat4 u_model = mat4(1.0);"
            "uniform mat4 u_view = mat4(1.0);"
            "uniform mat4 u_proj = mat4(1.0);"
            "uniform mat4 u_mvp = mat4(1.0);"
            "out vec3 pass_color;"
            "void main() {"
            "  pass_color = in_color;"
            "  gl_Position = u_mvp * vec4(in_pos, 1.0);"
            "}\0";

        const char *fshader =
            "#version 330 core\n"
            "in vec3 pass_color;"
            "void main() {"
            "  gl_FragColor = vec4(pass_color, 1.0);"
            "}\0";

        const char* sources[] = { vshader, fshader };
        const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        const char* uniforms[] = { "u_mvp" };
        const GLint buffer_lengths[] = { (GLint) strlen(vshader), (GLint) strlen(fshader) };
        shader = create_shader_program(sources, stages, sizeof(stages) / sizeof(GLenum), buffer_lengths);
    }

    void doRender() override {
        glEnable(GL_DEPTH_TEST);
        glUseProgram(shader);

        glm::mat4 model_matrix, projection_matrix;

        model_matrix = glm::scale(glm::mat4(1.0), glm::vec3(3.0, 0.125, 3.0));

        glm::mat4 view_matrix;
        get_view_matrix(&view_matrix, cam.pos, cam.pitch(), cam.yaw());

        float near_plane = 0.1f,
        far_plane = 100.0f;

        projection_matrix = glm::perspective((float) glm::radians(90.0), (float) view.width / (float) view.height, near_plane, far_plane);
        // projection_matrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, near_plane, far_plane);

        glUniformMatrix4fv(u_mvp, 1, GL_FALSE, &(projection_matrix * view_matrix * model_matrix)[0][0]);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);

        glDrawArrays(GL_TRIANGLES, 0, vbo.count);
    }

    void doPreTick(const runner_state& state) override {
        if (state.input.isKeyPressed(KEY_E))
            cam.pos.z -= 1.0;
        if (state.input.isKeyPressed(KEY_D))
            cam.pos.z += 1.0;
        if (state.input.isKeyPressed(KEY_S))
            cam.pos.x -= 1.0;
        if (state.input.isKeyPressed(KEY_F))
            cam.pos.x += 1.0;
    }

    void doTick(const runner_state& state) override {
        runtime += state.dt;
    }

};

camera3dRunner runner;

int main(const int argc, const char** argv) {
    debug_controller debug(WINDOW_WIDTH, WINDOW_HEIGHT);
    metrics_controller& metrics = debug.metrics;

    // register metrics
    auto tick_m     = metrics.init_metric(METRIC_FLOAT, "tick");
    auto text_m     = metrics.init_metric(METRIC_FLOAT, "text");
    auto render_m   = metrics.init_metric(METRIC_FLOAT, "render");

    // register averages
    auto render_a   = running_avg(20);

    auto timer = debug.create_timer();
    runner.init();
    debug.init();
    glfwSetKeyCallback(runner.window, [] (GLFWwindow* window, int key, int scancode, int action, int mods) {
        runner.handleKeyInput(key, scancode, action, mods);
    });

    printf("[INFO] Init time %g seconds.\n", debug.duration_milli(timer) / 1000.0f);

    double dt = 0.0;
    double time = 0.0;

    while (!glfwWindowShouldClose(runner.window)) {
        glfwPollEvents();

        dt = glfwGetTime() - time;
        time += dt;

        debug.reset_timer(timer);
        while (!runner.shouldRenderFrame())
            runner.tick(glfwGetTime());
        metrics.set(tick_m, (float) debug.duration_micro(timer));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        debug.reset_timer(timer);
        debug.render();
        runner.render();
        render_a.add(debug.duration_micro(timer));
        metrics.set(render_m, render_a.avg());

        debug.mesh_metric(tick_m);
        debug.mesh_metric(render_m);
        debug.mesh_metric(text_m);
        metrics.set(text_m, (float) debug.duration_micro(timer));
    }

    runner.shutdown();
    on_exit([] (int status, void* arg) {
        if (runner.window)
            glfwDestroyWindow(runner.window);
        glfwTerminate();
    }, nullptr);
    return 0;
}
