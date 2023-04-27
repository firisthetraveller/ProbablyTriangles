#include "TrackballCamera.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"

TrackballCamera::TrackballCamera(float distance, float angleX, float angleY)
    : _distance(distance), _angleX(angleX), _angleY(angleY){};

glm::mat4 TrackballCamera::getViewMatrix() const {
  static const glm::mat4 Identity = glm::mat4{1.f};
  glm::mat4 dMatrix = glm::translate(Identity, glm::vec3{0, 0, _distance});
  glm::mat4 xMatrix = glm::rotate(Identity, _angleX, glm::vec3{1, 0, 0});
  glm::mat4 yMatrix = glm::rotate(Identity, _angleY, glm::vec3{0, 1, 0});

  return dMatrix * xMatrix * yMatrix;
}
