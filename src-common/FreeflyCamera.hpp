#ifndef __FREEFLYC__
#define __FREEFLYC__

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/ext/vector_float3.hpp"
class FreeflyCamera {
private:
  glm::vec3 _position;
  float _phi;
  float _theta;

  glm::vec3 _frontVector;
  glm::vec3 _leftVector;
  glm::vec3 _upVector;

  void computeDirectionVectors();

public:
  /**
   * @param position : Initial position of the camera
   * @param phi : Lateral angle, in radians
   * @param theta : Vertical angle, in radians
   */
  FreeflyCamera(glm::vec3 position = glm::vec3{0, 0, 0},
                float phi = glm::pi<float>(), float theta = 0);
  glm::mat4 getViewMatrix() const;

  void moveLeft(float scalar);
  void moveFront(float scalar);
  void rotateLeft(float degrees);
  void rotateUp(float degrees);
};

#endif
