#include <memory>
#include <string>
#include <vector>

#include <napi.h>

#include "hnswlib.h"

class L2Space : public Napi::ObjectWrap<L2Space> {
public:
  uint32_t dim_;
  std::unique_ptr<hnswlib::L2Space> l2space_;

  L2Space(const Napi::CallbackInfo& info) : Napi::ObjectWrap<L2Space>(info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1) {
      Napi::Error::New(env, "wrong number of arguments (given " + std::to_string(info.Length()) + ", expected 1)")
        .ThrowAsJavaScriptException();
      return;
    }
    if (!info[0].IsNumber()) {
      Napi::TypeError::New(env, "wrong argument type, expected Number").ThrowAsJavaScriptException();
      return;
    }

    dim_ = info[0].As<Napi::Number>().Uint32Value();
    l2space_ = std::unique_ptr<hnswlib::L2Space>(new hnswlib::L2Space(static_cast<size_t>(this->dim_)));
  }

  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func =
      DefineClass(env, "L2Space", {InstanceMethod("distance", &L2Space::distance), InstanceMethod("dim", &L2Space::dim)});

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
      Napi::Error::New(env, "wrong number of arguments (given " + std::to_string(info.Length()) + ", expected 2)")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[0].IsArray()) {
      Napi::TypeError::New(env, "wrong first argument type, expected Array").ThrowAsJavaScriptException();
      return env.Null();
    }
    if (!info[1].IsArray()) {
      Napi::TypeError::New(env, "wrong second argument type, expected Array").ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Array arr_a = info[0].As<Napi::Array>();
    Napi::Array arr_b = info[1].As<Napi::Array>();

    if (arr_a.Length() != dim_) {
      Napi::Error::New(env, "wrong length of first array (given " + std::to_string(arr_a.Length()) + ", expected " +
                              std::to_string(dim_) + ")")
        .ThrowAsJavaScriptException();
      return env.Null();
    }
    if (arr_b.Length() != dim_) {
      Napi::Error::New(env, "wrong length of second array (given " + std::to_string(arr_b.Length()) + ", expected " +
                              std::to_string(dim_) + ")")
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

  Napi::Value dim(const Napi::CallbackInfo& info) { return Napi::Number::New(info.Env(), dim_); }
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  L2Space::Init(env, exports);
  return exports;
}

NODE_API_MODULE(addon, Init)
