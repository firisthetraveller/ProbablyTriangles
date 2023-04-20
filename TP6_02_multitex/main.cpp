#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include <cstdlib>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

GLuint loadTexture(img::Image &map) {
  GLuint tmp;

  glGenTextures(1, &tmp);
  glBindTexture(GL_TEXTURE_2D, tmp); // Binding de la texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, map.width(), map.height(), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, map.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  return tmp;
}

int main() {
  auto ctx = p6::Context{{1280, 720, "TP6 Sphère"}};
  ctx.maximize_window();

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/
  const std::vector<glimac::ShapeVertex> vertices =
      glimac::sphere_vertices(1.f, 32, 16);

  img::Image earthMap = p6::load_image_buffer("assets/textures/EarthMap.jpg");
  img::Image moonMap = p6::load_image_buffer("assets/textures/MoonMap.jpg");
  img::Image cloudMap = p6::load_image_buffer("assets/textures/CloudMap.jpg");
  // }
  // catch (std::exception &e) {
  //   std::cerr << "Couldn't load the texture\n";
  //   return EXIT_FAILURE;
  // }

  const p6::Shader shader =
      p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl");

  // Texture
  GLuint earthTexture = loadTexture(earthMap);
  GLuint moonTexture = loadTexture(moonMap);
  GLuint cloudTexture = loadTexture(cloudMap);

  // Uniform
  GLuint uMVPMatrix = glGetUniformLocation(shader.id(), "uMVPMatrix");
  GLuint uMVMatrix = glGetUniformLocation(shader.id(), "uMVMatrix");
  GLuint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
  GLuint uTexture1 = glGetUniformLocation(shader.id(), "uTexture1");
  GLuint uTexture2 = glGetUniformLocation(shader.id(), "uTexture2");

  if (uMVPMatrix < 0 || uMVMatrix < 0 || uNormalMatrix < 0) {
    std::cerr << "Uniform not found in shaders. Double check the names.\n";
    return EXIT_FAILURE;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Buffer initialization
  // VBO : Data
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  {
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex),
                 vertices.data(), GL_STATIC_DRAW);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // VAO : Attributes for the Vertex Shader
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  {
    const GLuint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

    const GLuint VERTEX_ATTR_NORMAL = 1;
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);

    const GLuint VERTEX_ATTR_UV_COORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_UV_COORDS);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    {
      // Attribute | Size | Variable size | ? | Size of one vertex complete data
      // | Offset
      glVertexAttribPointer(
          VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE,
          sizeof(glimac::ShapeVertex),
          (const void *)(offsetof(glimac::ShapeVertex, position)));
      glVertexAttribPointer(
          VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE,
          sizeof(glimac::ShapeVertex),
          (const void *)(offsetof(glimac::ShapeVertex, normal)));
      glVertexAttribPointer(
          VERTEX_ATTR_UV_COORDS, 2, GL_FLOAT, GL_FALSE,
          sizeof(glimac::ShapeVertex),
          (const void *)(offsetof(glimac::ShapeVertex, texCoords)));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);

  std::vector<glm::vec3> orbits;
  orbits.reserve(32);
  for (int i = 0; i < 32; i++) {
    orbits[i] = glm::sphericalRand(1.);
  }

  // Declare your infinite update loop.
  ctx.update = [&]() {
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/
    shader.use();

    // glClearColor(0.f, 0.75f, 0.75f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniform1i(uTexture1, 0);
    glUniform1i(uTexture2, 1);

    glBindVertexArray(vao);
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, earthTexture);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, cloudTexture);
      glm::mat4 ProjMatrix =
          glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
      glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
      glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

      glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE,
                         glm::value_ptr(ProjMatrix * MVMatrix));
      glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
      glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,
                         glm::value_ptr(NormalMatrix));
      glDrawArrays(GL_TRIANGLES, 0, vertices.size());

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, moonTexture);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, 0);

      for (int i = 0; i < 32; i++) {
        MVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
        MVMatrix = glm::rotate(MVMatrix, ctx.time(), orbits[i]);
        MVMatrix = glm::translate(MVMatrix, {-2.f, 0.f, 0.f});
        MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
      }
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindVertexArray(0);
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
