#include <algorithm>
#include "slice.h"

namespace sterling {

  
  void sort_model(Model& model) {
    auto sort_fn = [](Triangle& lhs, Triangle& rhs) {
      return lhs.max_z() < rhs.max_z();
    };
    std::sort(model.begin(), model.end(), sort_fn);
  }
  
  
};
