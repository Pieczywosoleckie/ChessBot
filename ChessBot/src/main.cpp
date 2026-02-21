#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* board_vert_src = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vColor;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vColor = aColor;
})";

const char* board_frag_src = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
})";

const char* piece_vert_src = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
out vec2 vTexCoord;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vTexCoord = aTexCoord;
})";

const char* piece_frag_src = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    FragColor = texture(uTexture, vTexCoord);
})";

GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    int ok;
    char log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("Shader error: %s\n", log);
    }
    return shader;
}

GLuint createProgram(const char* vert, const char* frag) {
    GLuint v = compileShader(GL_VERTEX_SHADER, vert);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, frag);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);
    int ok;
    char log[512];
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        glGetProgramInfoLog(prog, 512, NULL, log);
        printf("Program link error: %s\n", log);
    }
    glDeleteShader(v);
    glDeleteShader(f);
    return prog;
}

GLuint loadTexture(const char* path) {
    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &channels, 4); 
    if (!data) {
        printf("Nie mozna wczytac tekstury: %s\n", path);
        return 0;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return tex;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Szachownica 8x8", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n"); return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint boardProg = createProgram(board_vert_src, board_frag_src);
    GLuint pieceProg = createProgram(piece_vert_src, piece_frag_src);

    float step = 2.0f / 8.0f;
    std::vector<float> boardVerts;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            float x0 = -1.0f + col * step;
            float y0 = -1.0f + row * step;
            float x1 = x0 + step;
            float y1 = y0 + step;

            bool isLight = (row + col) % 2 == 0;
            float r = isLight ? 0.9f : 0.2f;
            float g = isLight ? 0.9f : 0.2f;
            float b = isLight ? 0.9f : 0.2f;

            float quad[] = {
                x0, y0, r, g, b,
                x1, y0, r, g, b,
                x1, y1, r, g, b,
                x0, y0, r, g, b,
                x1, y1, r, g, b,
                x0, y1, r, g, b
            };
            boardVerts.insert(boardVerts.end(), std::begin(quad), std::end(quad));
        }
    }

    GLuint boardVAO, boardVBO;
    glGenVertexArrays(1, &boardVAO);
    glGenBuffers(1, &boardVBO);
    glBindVertexArray(boardVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boardVBO);
    glBufferData(GL_ARRAY_BUFFER, boardVerts.size() * sizeof(float), boardVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    int kingCol = 4; 
    int kingRow = 0; 

    float kx0 = -1.0f + kingCol * step;
    float ky0 = -1.0f + kingRow * step;
    float kx1 = kx0 + step;
    float ky1 = ky0 + step;

    float pieceVerts[] = {
        kx0, ky0,  0.0f, 0.0f,
        kx1, ky0,  1.0f, 0.0f,
        kx1, ky1,  1.0f, 1.0f,

        kx0, ky0,  0.0f, 0.0f,
        kx1, ky1,  1.0f, 1.0f,
        kx0, ky1,  0.0f, 1.0f
    };

    GLuint pieceVAO, pieceVBO;
    glGenVertexArrays(1, &pieceVAO);
    glGenBuffers(1, &pieceVBO);
    glBindVertexArray(pieceVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pieceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pieceVerts), pieceVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    GLuint kingTex = loadTexture("../../img/whiteKing.png");

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(boardProg);
        glBindVertexArray(boardVAO);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(boardVerts.size() / 5));

        if (kingTex != 0) {
            glUseProgram(pieceProg);
            glBindTexture(GL_TEXTURE_2D, kingTex);
            glUniform1i(glGetUniformLocation(pieceProg, "uTexture"), 0);
            glBindVertexArray(pieceVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &boardVAO);
    glDeleteBuffers(1, &boardVBO);
    glDeleteVertexArrays(1, &pieceVAO);
    glDeleteBuffers(1, &pieceVBO);
    glDeleteTextures(1, &kingTex);
    glDeleteProgram(boardProg);
    glDeleteProgram(pieceProg);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}