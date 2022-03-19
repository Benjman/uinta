#include <glad/glad.h>

#include <cmath>
#include <cstdio>

#include <entt/entt.hpp>

#include <mesh.hpp>
#include <window.hpp>

#include "quadtreeRunner.hpp"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 in_pos;"
    "out vec3 pass_color;"
    "void main() {"
    "   gl_Position = vec4(in_pos.x, in_pos.y, 1.0, 1.0);"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;"
    "uniform vec3 u_color;"
    "void main() {"
    "   FragColor = vec4(u_color, 1.0);"
    "}\0";


void quadtreeRunner::init() {
    createWindow(width, height, "hello quadtree");
    squareWidth = (float) squareSize / window::width;
    squareHeight = (float) squareSize / window::height;

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("[ERROR] Vertex shader compilation failed.\n:%s\n", infoLog);
        throw std::exception();
    }

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("[ERROR] Fragment shader compilation failed.\n:%s\n", infoLog);
        throw std::exception();
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("[ERROR] Linking shader failed.\n:%s\n", infoLog);
        throw std::exception();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    uniformColor = glGetUniformLocation(shaderProgram, "u_color");

    unsigned int vbo, vao, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), 0, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), 0, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

struct square {

};

void quadtreeRunner::update(float runningTime) {
    // TODO accumulator 
    // TODO doUpdate so that runner can manage render times and update cycles.
    vertexCount = 0;
    indexCount = 0;
    indexOffset = 0;

    float cos_inner = (std::cos(runningTime * 0.13) + 2) * 0.25;
    float sin_inner = (std::sin(runningTime * 0.33) + 2) * 0.25;
    float cos_outer = (std::cos(runningTime * 0.25) + 1) * 0.5;
    float sin_outer = (std::sin(runningTime * 0.1) + 1) * 0.5;

    qt.clear();
    vec2 squarePos_inner = vec2(cos_inner * qt_width + qt.topLeftBounds.x, sin_inner * qt_height + qt.topLeftBounds.y);
    qt.insert((entt::entity) 1, squarePos_inner);

    vec2 squarePos_outer = vec2(cos_outer * qt_width + qt.topLeftBounds.x, sin_outer * qt_height + qt.topLeftBounds.y);
    qt.insert((entt::entity) 2, squarePos_outer);

    generateMesh(&qt, vertices, indices, &vertexCount, &indexCount, &indexOffset);

    // upload qt
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertexCount, vertices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * indexCount, indices);

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

    for (int i = 0; i < 12; i++) squareIndices[i] += indexOffset;

    // upload squares
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, sizeof(squareVertices), squareVertices);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, sizeof(squareIndices), squareIndices);
}

void quadtreeRunner::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniform3f(uniformColor, 1.0f, 0.5f, 0.2f);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glUniform3f(uniformColor, 0.0f, 1.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) (indexCount * sizeof(GLuint)));

    glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) ((indexCount + 6) * sizeof(GLuint)));

    glfwSwapBuffers(view.glfwWindow);
    glfwPollEvents();
}
