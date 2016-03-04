#pragma once

#include <array>
#include <deque>
#include <vector>
#include <memory>

namespace sterling {
  using vec3_store = std::array<double, 3>;
  using vec3 = std::shared_ptr<vec3_store>;
  using vec2_store = std::array<double, 2>;
  using vec2 = std::shared_ptr<vec2_store>;

  
  class Line {
  public:
    Line() = delete;
    Line(vec2, vec2);
    void print();
  private:
    vec2 start;
    vec2 end;
  };

  
  class Triangle {
  public:
    Triangle();
    bool full();
    void print();
    void add_vertex(vec3);
    double min_z();
    double max_z();
    Line intersection(double);
  private:
    std::vector<vec3> vertices;
    std::shared_ptr<double> memo_min_z;
    std::shared_ptr<double> memo_max_z;
  };

  using Model = std::deque<Triangle>;
  using Slice = std::deque<Line>;

  double mix(double, double, double);
  vec2 bisect(vec3, vec3, double);
  void print_model(Model);
  void print_slice(Slice);
  Model parse_stl(const char*);
};
