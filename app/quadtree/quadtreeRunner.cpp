#include <glad/glad.h>

#include <cmath>

#include "quadtreeRunner.hpp"

#include <mesh.hpp>
#include <shader.hpp>

void quadtreeRunner::init() {
    view.width = 1088;
    view.height = 1088;
    view.title = "hello quadtree";

    squareWidth = (float) squareSize / view.width;
    squareHeight = (float) squareSize / view.height;

    createGLFWWindow(view);
    init_shader();
    init_buffers();
}

void quadtreeRunner::init_buffers() {
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

void quadtreeRunner::init_shader() {
    const char *vshader = "#version 330 core\n"
        "layout (location = 0) in vec2 in_pos;"
        "out vec3 pass_color;"
        "void main() {"
        "   gl_Position = vec4(in_pos.x, in_pos.y, 1.0, 1.0);"
        "}\0";
    const char *fshader = "#version 330 core\n"
        "out vec4 FragColor;"
        "uniform vec3 u_color;"
        "void main() {"
        "   FragColor = vec4(u_color, 1.0);"
        "}\0";
    const char* sources[] = { vshader, fshader };
    const GLenum stages[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    const char* uniform_names[] = { "u_color" };
    GLuint shader = create_shader_program(sources, stages, 2,
                                          uniform_names, &uniformColor, 1);

}

void quadtreeRunner::tick(float runningTime) {
    vbo.count = 0;
    ebo.count = 0;
    ebo.offset = 0;

    float cos_inner = (std::cos(runningTime * 0.13) + 2) * 0.25;
    float sin_inner = (std::sin(runningTime * 0.33) + 2) * 0.25;
    float cos_outer = (std::cos(runningTime * 0.25) + 1) * 0.5;
    float sin_outer = (std::sin(runningTime * 0.1) + 1) * 0.5;

    qt.clear();

    vec2 squarePos_inner = vec2(cos_inner * qt_width + qt.topLeftBounds.x, sin_inner * qt_height + qt.topLeftBounds.y);
    qt.insert((entt::entity) 1, squarePos_inner);

    vec2 squarePos_outer = vec2(cos_outer * qt_width + qt.topLeftBounds.x, sin_outer * qt_height + qt.topLeftBounds.y);
    qt.insert((entt::entity) 2, squarePos_outer);

    generateMesh(&qt, vertices, indices, &vbo.count, &ebo.count, &ebo.offset, view.width, view.height);

    // upload qt
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vbo.count, vertices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * ebo.count, indices);

    // generate squares
    vec2 squareInnerNorm = squarePos_inner / vec2(width, height);
    squareInnerNorm.x =  2 * squareInnerNorm.x - 1;
    squareInnerNorm.y = -2 * squareInnerNorm.y + 1;

    vec2 squareOuterNorm = squarePos_outer / vec2(width, height);
    squareOuterNorm.x =  2 * squareOuterNorm.x - 1;
    squareOuterNorm.y = -2 * squareOuterNorm.y + 1;

    GLuint squareIndices[12] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4 };
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

void quadtreeRunner::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform3f(uniformColor, 1.0f, 0.5f, 0.2f);
    glDrawElements(GL_TRIANGLES, ebo.count, GL_UNSIGNED_INT, 0);

    glUniform3f(uniformColor, 0.0f, 1.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) (ebo.count * sizeof(GLuint)));

    glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) ((ebo.count + 6) * sizeof(GLuint)));

    glfwSwapBuffers(view.window);
}
