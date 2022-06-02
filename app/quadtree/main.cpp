#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <uinta/buffer.hpp>
#include <uinta/macros.hpp>
#include <uinta/mesh.hpp>
#include <uinta/quadtree.hpp>

#define UINTA_APP_UTILS_IMPL
#include "../utils/utils.hpp"

struct QuadtreeRunner final : GlfwRunner {
    unsigned int width = 1088;
    unsigned int height = 1088;

    GLfloat vertices[KILOBYTES(2)];
    GLuint indices[KILOBYTES(2)];

    Quad qt = Quad(glm::vec2(32), glm::vec2(1056), 16);
    float qt_width = qt.bottomRightBounds.x - qt.topLeftBounds.x;
    float qt_height = qt.bottomRightBounds.y - qt.topLeftBounds.y;

    unsigned int squareSize = 16;
    float squareWidth;
    float squareHeight;

    GLuint u_color = 0u;

    gl_buf vbo;
    gl_buf ebo;

    QuadtreeRunner() : GlfwRunner("hello quadtree", 1088, 1088) {
        squareWidth = (float) squareSize / display.width;
        squareHeight = (float) squareSize / display.height;
    }

    void doInit() override {
        initShader();
        initBuffers();
    }

    void initBuffers() {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        GLuint ids[2];
        glGenBuffers(2, ids);
        vbo.id = ids[0];
        ebo.id = ids[1];
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), 0, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), 0, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void initShader() {
        const char* vshader = "#version 330 core\n"
            "layout (location = 0) in vec2 in_pos;"
            "out vec3 pass_color;"
            "void main() {"
            "   gl_Position = vec4(in_pos.x, in_pos.y, 1.0, 1.0);"
            "}\0";
        const char* fshader = "#version 330 core\n"
            "out vec4 out_color;"
            "uniform vec3 u_color;"
            "void main() {"
            "   out_color = vec4(u_color, 1.0);"
            "}\0";
        const std::vector<std::string> sources({ std::string(vshader, strlen(vshader)), std::string(fshader, strlen(fshader)) });
        const std::vector<GLenum> stages({ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER });
        const std::vector<std::string> uniforms({ "u_color" });
        const std::vector<GLuint*> locations = { &u_color };
        createShaderProgram(sources, stages, uniforms, locations);
    }

    void doRender() override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform3f(u_color, 1.0f, 0.5f, 0.2f);
        glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);

        glUniform3f(u_color, 0.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) (ebo.count * sizeof(GLuint)));

        glUniform3f(u_color, 1.0f, 1.0f, 0.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) ((ebo.count + 6) * sizeof(GLuint)));
    }

    void doTick(const RunnerState& state) override {
        vbo.count = 0;
        ebo.count = 0;
        ebo.offset = 0;

        float cos_inner = (std::cos(state.runtime * 0.13) + 2) * 0.25;
        float sin_inner = (std::sin(state.runtime * 0.33) + 2) * 0.25;
        float cos_outer = (std::cos(state.runtime * 0.25) + 1) * 0.5;
        float sin_outer = (std::sin(state.runtime * 0.1) + 1) * 0.5;

        qt.clear();

        glm::vec2 squarePos_inner(cos_inner * qt_width + qt.topLeftBounds.x, sin_inner * qt_height + qt.topLeftBounds.y);
        qt.insert((entt::entity) 1, squarePos_inner);

        glm::vec2 squarePos_outer(cos_outer * qt_width + qt.topLeftBounds.x, sin_outer * qt_height + qt.topLeftBounds.y);
        qt.insert((entt::entity) 2, squarePos_outer);

        generateMesh(&qt, vertices, indices, &vbo.count, &ebo.count, &ebo.offset, display.width, display.height);

        // upload qt
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vbo.count, vertices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * ebo.count, indices);

        // generate squares
        glm::vec2 squareInnerNorm = squarePos_inner / glm::vec2(width, height);
        squareInnerNorm.x =  2 * squareInnerNorm.x - 1;
        squareInnerNorm.y = -2 * squareInnerNorm.y + 1;

        glm::vec2 squareOuterNorm = squarePos_outer / glm::vec2(width, height);
        squareOuterNorm.x =  2 * squareOuterNorm.x - 1;
        squareOuterNorm.y = -2 * squareOuterNorm.y + 1;

        GLuint squareIndices[] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
        GLfloat squareVertices[] = {
            squareInnerNorm.x - squareWidth * 0.5f, squareInnerNorm.y + squareHeight * 0.5f,
            squareInnerNorm.x - squareWidth * 0.5f, squareInnerNorm.y - squareHeight * 0.5f,
            squareInnerNorm.x + squareWidth * 0.5f, squareInnerNorm.y - squareHeight * 0.5f,
            squareInnerNorm.x + squareWidth * 0.5f, squareInnerNorm.y + squareHeight * 0.5f,

            squareOuterNorm.x - squareWidth * 0.5f, squareOuterNorm.y + squareHeight * 0.5f,
            squareOuterNorm.x - squareWidth * 0.5f, squareOuterNorm.y - squareHeight * 0.5f,
            squareOuterNorm.x + squareWidth * 0.5f, squareOuterNorm.y - squareHeight * 0.5f,
            squareOuterNorm.x + squareWidth * 0.5f, squareOuterNorm.y + squareHeight * 0.5f,
        };

        for (int i = 0; i < 12; i++)
            squareIndices[i] += ebo.offset;

        // upload squares
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vbo.count, sizeof(squareVertices), squareVertices);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ebo.count, sizeof(squareIndices), squareIndices);
    }

};

QuadtreeRunner runner;

int main(const int argc, const char **argv) {
    return runner.run();
}
