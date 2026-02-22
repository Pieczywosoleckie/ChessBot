#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <windows.h>

#include "chess.h"
#include "resource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char *board_vert_src = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vColor;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    vColor = aColor;
})";

const char *board_frag_src = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
})";

const char *piece_vert_src = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
uniform vec2 uOffset;
out vec2 vTexCoord;
void main() {
    gl_Position = vec4(aPos + uOffset, 0.0, 1.0);
    vTexCoord = aTexCoord;
})";

const char *piece_frag_src = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    FragColor = texture(uTexture, vTexCoord);
})";

GLuint compileShader(GLenum type, const char *src) {
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

GLuint createProgram(const char *vert, const char *frag) {
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

GLuint loadTextureFromResource(int resourceId) {
  HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(resourceId), RT_RCDATA);
  if (!hRes) {
    printf("Nie mozna znalezc zasobu: %d\n", resourceId);
    return 0;
  }
  HGLOBAL hData = LoadResource(NULL, hRes);
  if (!hData) {
    printf("Nie mozna zaladowac zasobu: %d\n", resourceId);
    return 0;
  }
  const unsigned char *pData = (const unsigned char *)LockResource(hData);
  DWORD dataSize = SizeofResource(NULL, hRes);

  int w, h, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *imgData =
      stbi_load_from_memory(pData, (int)dataSize, &w, &h, &channels, 4);
  if (!imgData) {
    printf("Nie mozna zdekodowac tekstury z zasobu: %d\n", resourceId);
    return 0;
  }
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               imgData);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(imgData);
  return tex;
}

GLuint getTextureForPiece(char piece, GLuint WhitetexK, GLuint WhitetexQ,
                          GLuint WhitetexR, GLuint WhitetexB, GLuint WhitetexN,
                          GLuint WhitetexP, GLuint BlacktexK, GLuint BlacktexQ,
                          GLuint BlacktexR, GLuint BlacktexB, GLuint BlacktexN,
                          GLuint BlacktexP) {
  switch (piece) {
  case 'K':
    return WhitetexK;
  case 'Q':
    return WhitetexQ;
  case 'R':
    return WhitetexR;
  case 'B':
    return WhitetexB;
  case 'N':
    return WhitetexN;
  case 'P':
    return WhitetexP;
  case 'k':
    return BlacktexK;
  case 'q':
    return BlacktexQ;
  case 'r':
    return BlacktexR;
  case 'b':
    return BlacktexB;
  case 'n':
    return BlacktexN;
  case 'p':
    return BlacktexP;
  default:
    return 0;
  }
}

bool isDragging = false;
int dragRow = -1;
int dragCol = -1;
float mouseNdcX = 0.0f;
float mouseNdcY = 0.0f;

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button != GLFW_MOUSE_BUTTON_LEFT)
    return;

  double mx, my;
  glfwGetCursorPos(window, &mx, &my);

  int winW, winH;
  glfwGetWindowSize(window, &winW, &winH);

  float ndcX = (float)(mx / winW) * 2.0f - 1.0f;
  float ndcY = 1.0f - (float)(my / winH) * 2.0f;

  float step = 2.0f / 8.0f;

  if (action == GLFW_PRESS) {
    int col = (int)((ndcX + 1.0f) / step);
    int row = (int)((ndcY + 1.0f) / step);

    if (col < 0)
      col = 0;
    if (col > 7)
      col = 7;
    if (row < 0)
      row = 0;
    if (row > 7)
      row = 7;

    int boardRow = 7 - row;

    Chess *chess = (Chess *)glfwGetWindowUserPointer(window);
    auto board = chess->getBoard();

    if (board[boardRow][col] != '.') {
      isDragging = true;
      dragRow = boardRow;
      dragCol = col;
      mouseNdcX = ndcX;
      mouseNdcY = ndcY;
    }
  } else if (action == GLFW_RELEASE) {
    if (isDragging && dragRow >= 0 && dragCol >= 0) {
      int toCol = (int)((ndcX + 1.0f) / step);
      int toRow = (int)((ndcY + 1.0f) / step);

      if (toCol < 0)
        toCol = 0;
      if (toCol > 7)
        toCol = 7;
      if (toRow < 0)
        toRow = 0;
      if (toRow > 7)
        toRow = 7;

      int toBoardRow = 7 - toRow;

      Chess *chess = (Chess *)glfwGetWindowUserPointer(window);
      auto board = chess->getBoard();
      char piece = board[dragRow][dragCol];

      std::pair<int, int> from = {dragRow, dragCol};
      std::pair<int, int> to = {toBoardRow, toCol};

      if (from != to) {
        bool moved = false;
        switch (piece) {
        case 'P':
			moved = chess->moveWhitePawn(from, to);
            break;
        case 'p':
          moved = chess->moveBlackPawn(from, to);
          break;
        case 'N':
            break;
        case 'n':
          moved = chess->moveBlackKnight(from, to);
          break;
        case 'B':
            break;
        case 'b':
          moved = chess->moveBlackBishop(from, to);
          break;
        case 'R':
            break;
        case 'r':
          moved = chess->moveBlackRook(from, to);
          break;
        case 'Q':
            break;
        case 'q':
          moved = chess->moveBlackQueen(from, to);
          break;
        case 'K':
            break;
        case 'k':
          moved = chess->moveBlackKing(from, to);
          break;
        }
        if (moved) {
          printf("Moved '%c' from (%d,%d) to (%d,%d)\n", piece, from.first,
                 from.second, to.first, to.second);
        } else {
          printf("Invalid move for '%c' from (%d,%d) to (%d,%d)\n", piece,
                 from.first, from.second, to.first, to.second);
        }
      }
    }
    isDragging = false;
    dragRow = -1;
    dragCol = -1;
  }
}

void cursor_pos_callback(GLFWwindow *window, double mx, double my) {
  int winW, winH;
  glfwGetWindowSize(window, &winW, &winH);
  mouseNdcX = (float)(mx / winW) * 2.0f - 1.0f;
  mouseNdcY = 1.0f - (float)(my / winH) * 2.0f;
}

int main() {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window =
      glfwCreateWindow(800, 800, "Szachownica 8x8", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, cursor_pos_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
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

      float quad[] = {x0, y0, r, g, b, x1, y0, r, g, b, x1, y1, r, g, b,
                      x0, y0, r, g, b, x1, y1, r, g, b, x0, y1, r, g, b};
      boardVerts.insert(boardVerts.end(), std::begin(quad), std::end(quad));
    }
  }

  GLuint boardVAO, boardVBO;
  glGenVertexArrays(1, &boardVAO);
  glGenBuffers(1, &boardVBO);
  glBindVertexArray(boardVAO);
  glBindBuffer(GL_ARRAY_BUFFER, boardVBO);
  glBufferData(GL_ARRAY_BUFFER, boardVerts.size() * sizeof(float),
               boardVerts.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  float pieceVerts[] = {
      0.0f, 0.0f, 0.0f, 0.0f, step, 0.0f, 1.0f, 0.0f, step, step, 1.0f, 1.0f,

      0.0f, 0.0f, 0.0f, 0.0f, step, step, 1.0f, 1.0f, 0.0f, step, 0.0f, 1.0f};

  GLuint pieceVAO, pieceVBO;
  glGenVertexArrays(1, &pieceVAO);
  glGenBuffers(1, &pieceVBO);
  glBindVertexArray(pieceVAO);
  glBindBuffer(GL_ARRAY_BUFFER, pieceVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pieceVerts), pieceVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  GLuint texKing = loadTextureFromResource(IDR_WHITE_KING);
  GLuint texQueen = loadTextureFromResource(IDR_WHITE_QUEEN);
  GLuint texRook = loadTextureFromResource(IDR_WHITE_ROOK);
  GLuint texBishop = loadTextureFromResource(IDR_WHITE_BISHOP);
  GLuint texKnight = loadTextureFromResource(IDR_WHITE_KNIGHT);
  GLuint texPawn = loadTextureFromResource(IDR_WHITE_PAWN);

  GLuint BlacktexKing = loadTextureFromResource(IDR_BLACK_KING);
  GLuint BlacktexQueen = loadTextureFromResource(IDR_BLACK_QUEEN);
  GLuint BlacktexRook = loadTextureFromResource(IDR_BLACK_ROOK);
  GLuint BlacktexBishop = loadTextureFromResource(IDR_BLACK_BISHOP);
  GLuint BlacktexKnight = loadTextureFromResource(IDR_BLACK_KNIGHT);
  GLuint BlacktexPawn = loadTextureFromResource(IDR_BLACK_PAWN);

  Chess chess;
  glfwSetWindowUserPointer(window, &chess);

  GLint offsetLoc = glGetUniformLocation(pieceProg, "uOffset");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(boardProg);
    glBindVertexArray(boardVAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(boardVerts.size() / 5));

    glUseProgram(pieceProg);
    glUniform1i(glGetUniformLocation(pieceProg, "uTexture"), 0);
    glBindVertexArray(pieceVAO);

    auto board = chess.getBoard();
    GLuint draggedTex = 0;

    for (int row = 0; row < 8; ++row) {
      for (int col = 0; col < 8; ++col) {
        char piece = board[row][col];
        if (piece == '.')
          continue;

        if (isDragging && row == dragRow && col == dragCol) {
          draggedTex = getTextureForPiece(
              piece, texKing, texQueen, texRook, texBishop, texKnight, texPawn,
              BlacktexKing, BlacktexQueen, BlacktexRook, BlacktexBishop,
              BlacktexKnight, BlacktexPawn);
          continue;
        }

        GLuint tex = getTextureForPiece(
            piece, texKing, texQueen, texRook, texBishop, texKnight, texPawn,
            BlacktexKing, BlacktexQueen, BlacktexRook, BlacktexBishop,
            BlacktexKnight, BlacktexPawn);

        if (tex == 0)
          continue;

        int glRow = 7 - row;
        int glCol = col;

        float offsetX = -1.0f + glCol * step;
        float offsetY = -1.0f + glRow * step;

        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform2f(offsetLoc, offsetX, offsetY);
        glDrawArrays(GL_TRIANGLES, 0, 6);
      }
    }

    if (isDragging && draggedTex != 0) {
      float halfStep = step / 2.0f;
      float offsetX = mouseNdcX - halfStep;
      float offsetY = mouseNdcY - halfStep;

      glBindTexture(GL_TEXTURE_2D, draggedTex);
      glUniform2f(offsetLoc, offsetX, offsetY);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &boardVAO);
  glDeleteBuffers(1, &boardVBO);
  glDeleteVertexArrays(1, &pieceVAO);
  glDeleteBuffers(1, &pieceVBO);
  glDeleteTextures(1, &texKing);
  glDeleteTextures(1, &texQueen);
  glDeleteTextures(1, &texRook);
  glDeleteTextures(1, &texBishop);
  glDeleteTextures(1, &texKnight);
  glDeleteTextures(1, &texPawn);
  glDeleteProgram(boardProg);
  glDeleteProgram(pieceProg);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}