#pragma once

#include <array>
#include <deque>
#include <vector>
#include <memory>

namespace sterling {
  using VertexStore = std::array<double, 3>;
  using Vertex = std::shared_ptr<VertexStore>;
  
  class Triangle {
  public:
    Triangle();
    bool full();
    void print();
    void add_vertex(Vertex);
    double max_z();
    std::vector<Vertex> vertices;
  private:
    std::shared_ptr<double> z_memo;
  };

  using Model = std::deque<Triangle>;

  void print_model(Model);
  Model parse_stl(const char*);
};
