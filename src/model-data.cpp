#include "model-data.h"
#include <algorithm>
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
  // class Line
  //
  Line::Line(vec2 vertex_a, vec2 vertex_b) {
    start = vertex_a;
    end = vertex_b;
  }

  void Line::print() {
    std::cout << "line:\n"
              << " - " << (*start)[0] << ", " << (*start)[1] << "\n"
              << " - " << (*end)[0] << ", " << (*end)[1] << "\n";
  }

  
  //
  // class Triangle
  //
  Triangle::Triangle() {
    auto vertices = std::vector<vec3>();
    memo_min_z = std::unique_ptr<double>();
    memo_max_z = std::unique_ptr<double>();
  }

  bool Triangle::full() {
    return vertices.size() == 3;
  }

  double Triangle::min_z() {
    if (!memo_min_z) {
      memo_min_z.reset(new double(std::min({
                (*vertices[0])[2],
                (*vertices[1])[2],
                (*vertices[2])[2]
                })));
    }
    return *memo_min_z;
  }

  double Triangle::max_z() {
    if (!memo_max_z) {
      memo_max_z.reset(new double(std::max({
                (*vertices[0])[2],
                (*vertices[1])[2],
                (*vertices[2])[2]
                })));
    }
    return *memo_max_z;
  }

  Line Triangle::intersection(double clip) {
    vec3 lowest;
    vec3 middle;
    vec3 highest;
    for (auto& point : vertices) {
      if ((*point)[2] == min_z() && !lowest) {
        lowest = point;
      }
      else if ((*point)[2] == max_z() && !highest) {
        highest = point;
      }
      else {
        middle = point;
      }
    }

    vec3 pivot = (*middle)[2] > clip ? lowest : highest;

    vec2&& start = bisect(lowest, highest, clip);
    vec2&& end = bisect(pivot, middle, clip);
  
    return Line(start, end);
  }

  void Triangle::add_vertex(vec3 v) {
    if (!full()) {
      vertices.push_back(v);
    }
  }

  void Triangle::print() {
    if (!full()) {
      std::cout << "tri( incomplete )\n";
    }
    else {
      std::cout << "tri: "
                <<"(min " << min_z() << ","
                <<" max " << max_z() << ")\n";
      for (auto& vert : vertices) {
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

  // linear interpolation
  double mix(double first, double second, double alpha) {
    return first + alpha * (second - first);
  }

  // create a 2D point between 3D points 'start' and 'end'
  vec2 bisect(vec3 start, vec3 end, double clip) {
    vec3 low;
    vec3 high;
    if ((*start)[2] < (*end)[2]) {
      low = start;
      high = end;
    }
    else {
      low = end;
      high = start;
    }
    double min_z = (*low)[2];
    double max_z = (*high)[2];
    double alpha = (clip-min_z) / (max_z-min_z);

    double new_x = mix((*low)[0], (*high)[0], alpha);
    double new_y = mix((*low)[0], (*high)[0], alpha);
    return vec2(new vec2_store({new_x, new_y}));
  }

  // print out a list of triangles
  void print_model(Model m) {
    for (auto& tri : m) {
      tri.print();
      std::cout << "\n";
    }
  }

  // print out the coordinates of the line segments in a slice
  void print_slice(Slice s) {
    for (auto& line : s) {
      line.print();
      std::cout << "\n";
    }
  }

  // for the given STL vertex line, parse out the vertex data
  vec3 find_verts(string line) {
    int i = 0;
    vec3_store* store = new vec3_store();
    std::smatch matched;
    while(std::regex_search(line, matched, pattern)) {
      double axis = std::stod(matched[0]);
      (*store)[i] = axis;
      i+=1;
      line = matched.suffix().str();
    }
    return vec3(store);
  }

  // produce a list of vertices from an ascii encoded STL model file
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
