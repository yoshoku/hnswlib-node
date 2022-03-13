#include <memory>
#include <new>
#include <string>
#include <vector>

#include <napi.h>

#include "hnswlib/hnswlib.h"

class L2Space : public Napi::ObjectWrap<L2Space> {
public:
  uint32_t dim_;
  std::unique_ptr<hnswlib::L2Space> l2space_;

  L2Space(const Napi::CallbackInfo& info) : Napi::ObjectWrap<L2Space>(info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return;
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return;
    }

    dim_ = info[0].As<Napi::Number>().Uint32Value();
    l2space_ = std::unique_ptr<hnswlib::L2Space>(new hnswlib::L2Space(static_cast<size_t>(dim_)));
  }

  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // clang-format off
    Napi::Function func = DefineClass(env, "L2Space", {
      InstanceMethod("distance", &L2Space::distance),
      InstanceMethod("getNumDimensions", &L2Space::getNumDimensions)
    });
    // clang-format on

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("L2Space", func);
    return exports;
  }

private:
  Napi::Value distance(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsArray()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr_a = info[0].As<Napi::Array>();
    Napi::Array arr_b = info[1].As<Napi::Array>();

    if (arr_a.Length() != dim_) {
      Napi::Error::New(env, "Invalid the first array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr_a.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (arr_b.Length() != dim_) {
      Napi::Error::New(env, "Invalid the second array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr_b.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<float> vec_a(dim_, 0.0);
    std::vector<float> vec_b(dim_, 0.0);

    for (uint32_t i = 0; i < dim_; i++) {
      Napi::Value val_a = arr_a[i];
      Napi::Value val_b = arr_b[i];
      vec_a[i] = val_a.As<Napi::Number>().FloatValue();
      vec_b[i] = val_b.As<Napi::Number>().FloatValue();
    }

    hnswlib::DISTFUNC<float> df = l2space_->get_dist_func();
    const float d = df(vec_a.data(), vec_b.data(), l2space_->get_dist_func_param());
    return Napi::Number::New(info.Env(), d);
  }

  Napi::Value getNumDimensions(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), dim_); }
};

class InnerProductSpace : public Napi::ObjectWrap<InnerProductSpace> {
public:
  uint32_t dim_;
  std::unique_ptr<hnswlib::InnerProductSpace> ipspace_;

  InnerProductSpace(const Napi::CallbackInfo& info) : Napi::ObjectWrap<InnerProductSpace>(info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return;
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return;
    }

    dim_ = info[0].As<Napi::Number>().Uint32Value();
    ipspace_ = std::unique_ptr<hnswlib::InnerProductSpace>(new hnswlib::InnerProductSpace(static_cast<size_t>(dim_)));
  }

  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // clang-format off
    Napi::Function func = DefineClass(env, "InnerProductSpace", {
      InstanceMethod("distance", &InnerProductSpace::distance),
      InstanceMethod("getNumDimensions", &InnerProductSpace::getNumDimensions)
    });
    // clang-format on

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("InnerProductSpace", func);
    return exports;
  }

private:
  Napi::Value distance(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsArray()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr_a = info[0].As<Napi::Array>();
    Napi::Array arr_b = info[1].As<Napi::Array>();

    if (arr_a.Length() != dim_) {
      Napi::Error::New(env, "Invalid the first array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr_a.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (arr_b.Length() != dim_) {
      Napi::Error::New(env, "Invalid the second array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr_b.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<float> vec_a(dim_, 0.0);
    std::vector<float> vec_b(dim_, 0.0);

    for (uint32_t i = 0; i < dim_; i++) {
      Napi::Value val_a = arr_a[i];
      Napi::Value val_b = arr_b[i];
      vec_a[i] = val_a.As<Napi::Number>().FloatValue();
      vec_b[i] = val_b.As<Napi::Number>().FloatValue();
    }

    hnswlib::DISTFUNC<float> df = ipspace_->get_dist_func();
    const float d = df(vec_a.data(), vec_b.data(), ipspace_->get_dist_func_param());
    return Napi::Number::New(info.Env(), d);
  }

  Napi::Value getNumDimensions(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), dim_); }
};

class BruteforceSearch : public Napi::ObjectWrap<BruteforceSearch> {
public:
  uint32_t dim_;
  uint32_t max_elements_;
  hnswlib::BruteforceSearch<float>* index_;
  hnswlib::SpaceInterface<float>* space_;

  BruteforceSearch(const Napi::CallbackInfo& info)
      : Napi::ObjectWrap<BruteforceSearch>(info), index_(nullptr), space_(nullptr) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return;
    }
    if (!info[0].IsString()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a string.").ThrowAsJavaScriptException();
      return;
    }
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be a number.").ThrowAsJavaScriptException();
      return;
    }

    const std::string space_name = info[0].As<Napi::String>().ToString();
    if (space_name != "l2" && space_name != "ip") {
      Napi::Error::New(env, "Wrong space name, expected \"l2\" or \"ip\".").ThrowAsJavaScriptException();
      return;
    }

    dim_ = info[1].As<Napi::Number>().Uint32Value();

    try {
      if (space_name == "l2") {
        space_ = new hnswlib::L2Space(static_cast<size_t>(dim_));
      } else {
        space_ = new hnswlib::InnerProductSpace(static_cast<size_t>(dim_));
      }
    } catch (const std::bad_alloc& err) {
      Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
      return;
    }

    max_elements_ = 0;
  }

  ~BruteforceSearch() {
    if (space_) delete space_;
    if (index_) delete index_;
  }

  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // clang-format off
    Napi::Function func = DefineClass(env, "BruteforceSearch", {
      InstanceMethod("initIndex", &BruteforceSearch::initIndex),
      InstanceMethod("loadIndex", &BruteforceSearch::loadIndex),
      InstanceMethod("saveIndex", &BruteforceSearch::saveIndex),
      InstanceMethod("addPoint", &BruteforceSearch::addPoint),
      InstanceMethod("removePoint", &BruteforceSearch::removePoint),
      InstanceMethod("searchKnn", &BruteforceSearch::searchKnn),
      InstanceMethod("getMaxElements", &BruteforceSearch::getMaxElements),
      InstanceMethod("getCurrentCount", &BruteforceSearch::getCurrentCount)
    });
    // clang-format on

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData<Napi::FunctionReference>(constructor);

    exports.Set("BruteforceSearch", func);
    return exports;
  }

private:
  Napi::Value initIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    max_elements_ = info[0].As<Napi::Number>().Uint32Value();

    if (index_) delete index_;

    try {
      index_ = new hnswlib::BruteforceSearch<float>(space_, static_cast<size_t>(max_elements_));
    } catch (const std::bad_alloc& err) {
      index_ = nullptr;
      Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value loadIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsString()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a string.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const std::string filename = info[0].As<Napi::String>().ToString();

    if (index_->data_) free(index_->data_);

    try {
      index_->loadIndex(filename, space_);
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value saveIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsString()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a string.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const std::string filename = info[0].As<Napi::String>().ToString();

    index_->saveIndex(filename);

    return env.Null();
  }

  Napi::Value addPoint(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr = info[0].As<Napi::Array>();
    if (arr.Length() != dim_) {
      Napi::Error::New(env, "Invalid the given array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<float> vec(dim_, 0.0);
    for (uint32_t i = 0; i < dim_; i++) {
      Napi::Value val = arr[i];
      vec[i] = val.As<Napi::Number>().FloatValue();
    }

    const uint32_t idx = info[1].As<Napi::Number>().Uint32Value();

    try {
      index_->addPoint(reinterpret_cast<void*>(vec.data()), static_cast<hnswlib::labeltype>(idx));
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value removePoint(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t idx = info[0].As<Napi::Number>().Uint32Value();

    index_->removePoint(static_cast<hnswlib::labeltype>(idx));

    return env.Null();
  }

  Napi::Value searchKnn(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr = info[0].As<Napi::Array>();
    if (arr.Length() != dim_) {
      Napi::Error::New(env, "Invalid the given array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t k = info[1].As<Napi::Number>().Uint32Value();
    if (k > max_elements_) {
      Napi::Error::New(env, "Invalid the number of k-nearest neighbors (cannot be given a value greater than `maxElements`: " +
                              std::to_string(max_elements_) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (k <= 0) {
      Napi::Error::New(env, "Invalid the number of k-nearest neighbors (must be a positive number).")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<float> vec(dim_, 0.0);
    for (uint32_t i = 0; i < dim_; i++) {
      Napi::Value val = arr[i];
      vec[i] = val.As<Napi::Number>().FloatValue();
    }
    std::priority_queue<std::pair<float, size_t>> knn =
      index_->searchKnn(reinterpret_cast<void*>(vec.data()), static_cast<size_t>(k));
    const size_t n_results = knn.size();
    Napi::Array arr_distances = Napi::Array::New(env, n_results);
    Napi::Array arr_neighbors = Napi::Array::New(env, n_results);
    for (int32_t i = static_cast<int32_t>(n_results) - 1; i >= 0; i--) {
      const std::pair<float, size_t>& nn = knn.top();
      arr_distances[i] = Napi::Number::New(env, nn.first);
      arr_neighbors[i] = Napi::Number::New(env, nn.second);
      knn.pop();
    }

    Napi::Object results = Napi::Object::New(env);
    results.Set("distances", arr_distances);
    results.Set("neighbors", arr_neighbors);
    return results;
  }

  Napi::Value getMaxElements(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (index_ == nullptr) return Napi::Number::New(env, 0);
    return Napi::Number::New(env, index_->maxelements_);
  }

  Napi::Value getCurrentCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (index_ == nullptr) return Napi::Number::New(env, 0);
    return Napi::Number::New(env, index_->cur_element_count);
  }
};

class HierarchicalNSW : public Napi::ObjectWrap<HierarchicalNSW> {
public:
  uint32_t dim_;
  hnswlib::HierarchicalNSW<float>* index_;
  hnswlib::SpaceInterface<float>* space_;

  HierarchicalNSW(const Napi::CallbackInfo& info) : Napi::ObjectWrap<HierarchicalNSW>(info), index_(nullptr), space_(nullptr) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return;
    }
    if (!info[0].IsString()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a string.").ThrowAsJavaScriptException();
      return;
    }
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be a number.").ThrowAsJavaScriptException();
      return;
    }

    const std::string space_name = info[0].As<Napi::String>().ToString();
    if (space_name != "l2" && space_name != "ip") {
      Napi::Error::New(env, "Wrong space name, expected \"l2\" or \"ip\".").ThrowAsJavaScriptException();
      return;
    }

    dim_ = info[1].As<Napi::Number>().Uint32Value();

    try {
      if (space_name == "l2") {
        space_ = new hnswlib::L2Space(static_cast<size_t>(dim_));
      } else {
        space_ = new hnswlib::InnerProductSpace(static_cast<size_t>(dim_));
      }
    } catch (const std::bad_alloc& err) {
      Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
      return;
    }
  }

  ~HierarchicalNSW() {
    if (space_) delete space_;
    if (index_) delete index_;
  }

  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    // clang-format off
    Napi::Function func = DefineClass(env, "HierarchicalNSW", {
      InstanceMethod("initIndex", &HierarchicalNSW::initIndex),
      InstanceMethod("loadIndex", &HierarchicalNSW::loadIndex),
      InstanceMethod("saveIndex", &HierarchicalNSW::saveIndex),
      InstanceMethod("resizeIndex", &HierarchicalNSW::resizeIndex),
      InstanceMethod("addPoint", &HierarchicalNSW::addPoint),
      InstanceMethod("markDelete", &HierarchicalNSW::markDelete),
      InstanceMethod("unmarkDelete", &HierarchicalNSW::unmarkDelete),
      InstanceMethod("searchKnn", &HierarchicalNSW::searchKnn),
      InstanceMethod("getIdsList", &HierarchicalNSW::getIdsList),
      InstanceMethod("getMaxElements", &HierarchicalNSW::getMaxElements),
      InstanceMethod("getCurrentCount", &HierarchicalNSW::getCurrentCount),
      InstanceMethod("getEf", &HierarchicalNSW::getEf),
      InstanceMethod("setEf", &HierarchicalNSW::setEf)
    });
    // clang-format on

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData<Napi::FunctionReference>(constructor);

    exports.Set("HierarchicalNSW", func);
    return exports;
  }

private:
  Napi::Value initIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
      Napi::Error::New(env, "Expected 1-4 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsUndefined() && !info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the sencond argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[2].IsUndefined() && !info[2].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the third argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[3].IsUndefined() && !info[3].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the fourth argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t max_elements_ = info[0].As<Napi::Number>().Uint32Value();
    const uint32_t m_ = info[1].IsUndefined() ? 16 : info[1].As<Napi::Number>().Uint32Value();
    const uint32_t ef_construction_ = info[2].IsUndefined() ? 200 : info[2].As<Napi::Number>().Uint32Value();
    const uint32_t random_seed_ = info[3].IsUndefined() ? 100 : info[3].As<Napi::Number>().Uint32Value();

    if (index_) delete index_;

    try {
      index_ = new hnswlib::HierarchicalNSW<float>(space_, static_cast<size_t>(max_elements_), static_cast<size_t>(m_),
                                                   static_cast<size_t>(ef_construction_), static_cast<size_t>(random_seed_));
    } catch (const std::bad_alloc& err) {
      index_ = nullptr;
      Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value loadIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsString()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a string.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const std::string filename = info[0].As<Napi::String>().ToString();

    if (index_) delete index_;

    try {
      index_ = new hnswlib::HierarchicalNSW<float>(space_, filename, false);
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    } catch (const std::bad_alloc& err) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(err.what())).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value saveIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsString()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a string.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const std::string filename = info[0].As<Napi::String>().ToString();

    index_->saveIndex(filename);

    return env.Null();
  }

  Napi::Value resizeIndex(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t new_max_elements = info[0].As<Napi::Number>().Uint32Value();

    try {
      index_->resizeIndex(static_cast<size_t>(new_max_elements));
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    } catch (const std::bad_alloc& err) {
      Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value addPoint(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr = info[0].As<Napi::Array>();
    if (arr.Length() != dim_) {
      Napi::Error::New(env, "Invalid the given array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<float> vec(dim_, 0.0);
    for (uint32_t i = 0; i < dim_; i++) {
      Napi::Value val = arr[i];
      vec[i] = val.As<Napi::Number>().FloatValue();
    }

    const uint32_t idx = info[1].As<Napi::Number>().Uint32Value();

    try {
      index_->addPoint(reinterpret_cast<void*>(vec.data()), static_cast<hnswlib::labeltype>(idx));
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value markDelete(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t idx = info[0].As<Napi::Number>().Uint32Value();

    try {
      index_->markDelete(static_cast<hnswlib::labeltype>(idx));
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value unmarkDelete(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t idx = info[0].As<Napi::Number>().Uint32Value();

    try {
      index_->unmarkDelete(static_cast<hnswlib::labeltype>(idx));
    } catch (const std::runtime_error& e) {
      Napi::Error::New(env, "Hnswlib Error: " + std::string(e.what())).ThrowAsJavaScriptException();
      return env.Null();
    }

    return env.Null();
  }

  Napi::Value searchKnn(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2) {
      Napi::Error::New(env, "Expected 2 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be an Array.").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the second argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr = info[0].As<Napi::Array>();
    if (arr.Length() != dim_) {
      Napi::Error::New(env, "Invalid the given array length (expected " + std::to_string(dim_) + ", but got " +
                              std::to_string(arr.Length()) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t k = info[1].As<Napi::Number>().Uint32Value();
    if (k > index_->max_elements_) {
      Napi::Error::New(env, "Invalid the number of k-nearest neighbors (cannot be given a value greater than `maxElements`: " +
                              std::to_string(index_->max_elements_) + ").")
        .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::vector<float> vec(dim_, 0.0);
    for (uint32_t i = 0; i < dim_; i++) {
      Napi::Value val = arr[i];
      vec[i] = val.As<Napi::Number>().FloatValue();
    }
    std::priority_queue<std::pair<float, size_t>> knn =
      index_->searchKnn(reinterpret_cast<void*>(vec.data()), static_cast<size_t>(k));
    const size_t n_results = knn.size();
    Napi::Array arr_distances = Napi::Array::New(env, n_results);
    Napi::Array arr_neighbors = Napi::Array::New(env, n_results);
    for (int32_t i = static_cast<int32_t>(n_results) - 1; i >= 0; i--) {
      const std::pair<float, size_t>& nn = knn.top();
      arr_distances[i] = Napi::Number::New(env, nn.first);
      arr_neighbors[i] = Napi::Number::New(env, nn.second);
      knn.pop();
    }

    Napi::Object results = Napi::Object::New(env);
    results.Set("distances", arr_distances);
    results.Set("neighbors", arr_neighbors);
    return results;
  }

  Napi::Value getIdsList(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (index_ == nullptr) return Napi::Array::New(env, 0);
    uint32_t n_elements = index_->label_lookup_.size();
    Napi::Array ids = Napi::Array::New(env, n_elements);
    uint32_t counter = 0;
    for (auto kv : index_->label_lookup_) {
      ids[counter] = Napi::Number::New(env, kv.first);
      counter++;
    }
    return ids;
  }

  Napi::Value getMaxElements(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (index_ == nullptr) return Napi::Number::New(env, 0);
    return Napi::Number::New(env, index_->max_elements_);
  }

  Napi::Value getCurrentCount(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (index_ == nullptr) return Napi::Number::New(env, 0);
    return Napi::Number::New(env, index_->cur_element_count);
  }

  Napi::Value getEf(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }
    return Napi::Number::New(env, index_->ef_);
  }

  Napi::Value setEf(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "Expected 1 arguments, but got " + std::to_string(info.Length()) + ".")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "Invalid the first argument type, must be a number.").ThrowAsJavaScriptException();
      return env.Null();
    }

    if (index_ == nullptr) {
      Napi::Error::New(env, "Search index has not been initialized, call `initIndex` in advance.").ThrowAsJavaScriptException();
      return env.Null();
    }

    const uint32_t ef = info[0].As<Napi::Number>().Uint32Value();
    index_->setEf(static_cast<size_t>(ef));

    return env.Null();
  }
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  L2Space::Init(env, exports);
  InnerProductSpace::Init(env, exports);
  BruteforceSearch::Init(env, exports);
  HierarchicalNSW::Init(env, exports);
  return exports;
}

NODE_API_MODULE(addon, Init)
