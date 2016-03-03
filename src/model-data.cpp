#include "model-data.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <regex>

using std::fstream;
using std::string;

namespace sterling {
  std::regex pattern("[0-9-.]+");

  
  //
  // class Triangle
  //
  Triangle::Triangle() {
    auto vertices = std::vector<Vertex>();
  }

  bool Triangle::full() {
    return vertices.size() == 3;
  }

  void Triangle::add_vertex(Vertex v) {
    if (!full()) {
      vertices.push_back(v);
    }
  }

  void Triangle::print() {
    if (!full()) {
      std::cout << "tri( incomplete )\n";
    }
    else {
      std::cout << "tri:\n";
      for (auto vert : vertices) {
        std::cout << " - "
                  << (*vert)[0] << ", "
                  << (*vert)[1] << ", "
                  << (*vert)[2]
                  << "\n";
      }
    }
  }


  

  //
  // stl parsing methods follow
  //

  void print_model(Model m) {
    for (auto tri : m) {
      tri.print();
      std::cout << "\n";
    }
  }
  
  Vertex find_verts(string line) {
    int i = 0;
    VertexStore* store = new VertexStore();
    std::smatch matched;
    while(std::regex_search(line, matched, pattern)) {
      double axis = std::stod(matched[0]);
      (*store)[i] = axis;
      i+=1;
      line = matched.suffix().str();
    }
    return Vertex(store);
  }
  
  Model parse_stl(const char* path) {
    fstream input;
    string line;
    input.open(path, std::ios::in);
    std::cout << "Reading in model data from " << path << "\n";

    auto imported = Model();
    auto builder = std::unique_ptr<Triangle>(new Triangle());
    
    while(std::getline(input, line)) {
      if (line[0] == 'v') {
        auto vert = find_verts(line);
        builder->add_vertex(vert);
        if (builder->full()) {
          imported.push_back(*builder);
          builder.reset(new Triangle());
        }
      }
    }
    input.close();
    return imported;
  }
  
};
