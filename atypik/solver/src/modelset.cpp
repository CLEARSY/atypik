/*
 * A TYPe Inference Kit for B.
 *
 * This file is part of the atypik project.
 * Copyright (c) 2023 CLEARSY
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>. 
 *
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the atypik software without
 * disclosing the source code of your own applications.
 *
 */
#include "modelset.h"

#include <future>
#include <thread>

using std::async;
using std::future;
using std::shared_future;
using std::string;
using std::thread;
using std::unordered_map;
using std::vector;

namespace solver {
ModelSet::ModelSet(vector<Model::shared_ptr> models) : models_(models) {}

unordered_map<Variable::shared_ptr, string> ModelSet::solve() {
  unordered_map<Variable::shared_ptr, string> result;
  vector<future<unordered_map<Variable::shared_ptr, string>>>
      model_future_results;
  model_future_results.reserve(models_.size());

  for (auto& model : models_) {
    model_future_results.push_back(std::async(&Model::solve, model));
  }

  for (auto& future_result : model_future_results) {
    result.merge(future_result.get());
  }

  return result;
}

Model::shared_ptr ModelSet::getModel(int num) { return models_[num]; }

}  // namespace solver
