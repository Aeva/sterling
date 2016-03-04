#pragma once
#include <memory>
#include "model-data.h"

namespace sterling {
  void sort_model(Model&);
  Slice find_intersection(Model&, double);
};
