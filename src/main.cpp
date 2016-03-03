#include <iostream>
#include "model-data.h"

using namespace sterling;

int main() {
  auto input = parse_stl("test.stl");
  print_model(input);

  return 0;
}
