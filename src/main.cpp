#include <iostream>
#include "model-data.h"
#include "slice.h"

using namespace sterling;

int main() {
  Model&& input = parse_stl("test.stl");
  sort_model(input);
  print_model(input);

  Slice&& slice = find_intersection(input, 5.0);
  print_slice(slice);

  return 0;
}
