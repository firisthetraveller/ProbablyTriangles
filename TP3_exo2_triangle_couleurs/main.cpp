#include "p6/p6.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

int main() {
  auto ctx = p6::Context{{1280, 720, "TP3 EX2"}};
  ctx.maximize_window();

  struct Vertex2DColor {
    glm::vec2 _position;
    glm::vec3 _color;

    Vertex2DColor(glm::vec2 position = {0, 0}, glm::vec3 color = {0, 0, 0})
        : _position(position), _color(color) {}
  };

  struct Square {
    std::vector<Vertex2DColor> _points;

    Square(Vertex2DColor a, Vertex2DColor b, Vertex2DColor c, Vertex2DColor d) {
      _points = std::vector<Vertex2DColor>();
      _points.emplace_back(a);
      _points.emplace_back(b);
      _points.emplace_back(c);
      _points.emplace_back(d);
    }

    std::vector<Vertex2DColor> points() {
      return {_points[0], _points[1], _points[2],
              _points[1], _points[2], _points[3]};
    }

    std::vector<uint32_t> indices() { return {0, 1, 2, 1, 2, 3}; }
  };

  struct Triangle {
    std::vector<Vertex2DColor> _points;

    Triangle(Vertex2DColor a, Vertex2DColor b, Vertex2DColor c) {
      _points = std::vector<Vertex2DColor>();
      _points.emplace_back(a);
      _points.emplace_back(b);
      _points.emplace_back(c);
    }

    std::vector<Vertex2DColor> points() {
      return {_points[0], _points[1], _points[2]};
    }

    std::vector<uint32_t> indices() { return {0, 1, 2}; }
  };

  struct Circle {
    std::vector<Vertex2DColor> _points;

    Circle(Vertex2DColor center, float radius, uint n) {
      _points = std::vector<Vertex2DColor>();
      _points.emplace_back(center);

      for (uint i = 0; i < n; i++) {
        float x = radius * std::cos(2 * i * glm::pi<float>() / n);
        float y = radius * std::sin(2 * i * glm::pi<float>() / n);
        std::cout << "(" << x << ", " << y << ")\n";
        _points.emplace_back(Vertex2DColor(glm::vec2(x, y), center._color));
      }
    }

    std::vector<Vertex2DColor> points() {
      std::vector<Vertex2DColor> tmp = std::vector<Vertex2DColor>();
      for (uint i = 0; i < _points.size() - 1; i++) {
        tmp.emplace_back(_points[0]);
        tmp.emplace_back(_points[i + 1]);
        std::cout << 0 << ' ' << i + 1;
        tmp.emplace_back(_points[((i + 1) % (_points.size() - 1)) + 1]);
        std::cout << " " << (i + 1) % (_points.size() - 1) + 1 << '\n';
      }
      std::cout << tmp.size() << '\n';
      return tmp;
    }

    std::vector<uint32_t> indices() {
      std::vector<uint32_t> tmp = std::vector<uint32_t>();
      for (uint32_t i = 0; i < _points.size() - 1; i++) {
        tmp.emplace_back(0);
        tmp.emplace_back(i);
        std::cout << 0 << ' ' << i + 1;
        tmp.emplace_back((i + 1) % (_points.size() - 1) + 1);
        std::cout << " " << (i + 1) % (_points.size() - 1) + 1 << '\n';
      }
      std::cout << tmp.size() << '\n';
      return tmp;
    }
  };
  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  // Declare your infinite update loop.
  ctx.update = [&]() {
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/
  };

  // Should be done last. It starts the infinite loop.
  ctx.start();
}
