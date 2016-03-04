#include <algorithm>
#include "slice.h"

namespace sterling {

  
  void sort_model(Model& model) {
    auto sort_fn = [](Triangle& lhs, Triangle& rhs) {
      return lhs.max_z() < rhs.max_z();
    };
    std::sort(model.begin(), model.end(), sort_fn);
  }


  Slice find_intersection(Model& model, double clip) {
    auto intersection = Slice();
    for (auto& tri : model) {
      if (tri.min_z() <= clip && clip <= tri.max_z()) {
         auto segment = tri.intersection(clip);
         intersection.push_back(segment);
      }
      else if (tri.max_z() > clip) {
        break;
      }
    }
    return intersection;
  }
  
};
