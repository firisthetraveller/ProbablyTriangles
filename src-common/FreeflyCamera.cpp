#include "FreeflyCamera.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/geometric.hpp"

FreeflyCamera::FreeflyCamera(glm::vec3 position, float phi, float theta)
    : _position(position), _phi(phi), _theta(theta) {
  computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors() {
  _frontVector = {glm::cos(_theta) * glm::sin(_phi), glm::sin(_theta),
                  glm::cos(_theta) * glm::cos(_phi)};
  _leftVector = {glm::sin(_phi + glm::pi<float>() / 2.f), 0.f,
                 glm::cos(_phi + glm::pi<float>() / 2.f)};
  _upVector = glm::cross(_frontVector, _leftVector);
}

glm::mat4 FreeflyCamera::getViewMatrix() const {
  return glm::lookAt(_position, _position + _frontVector, _upVector);
}

void FreeflyCamera::moveLeft(float scalar) {
  _position += scalar * _leftVector;
}

void FreeflyCamera::moveFront(float scalar) {
  _position += scalar * _frontVector;
}

void FreeflyCamera::rotateLeft(float degrees) {
  _theta += degrees * glm::pi<float>() / 180;
  computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees) {
  _phi += degrees * glm::pi<float>() / 180;
  computeDirectionVectors();
}
