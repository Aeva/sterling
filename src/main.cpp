#include <iostream>
#include "model-data.h"
#include "slice.h"

using namespace sterling;

int main() {
  auto input = parse_stl("test.stl");
  sort_model(input);
  print_model(input);

  return 0;
}
