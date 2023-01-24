#include "react-native-multithreadding-lite.h"
#include "ThreadPool.h"

using namespace std;
using namespace facebook;

namespace multithreaddinglite {
std::shared_ptr<react::CallInvoker> invoker;

double multiply(double a, double b) {
  return a * b;
}

void install(jsi::Runtime &runtime, std::shared_ptr<react::CallInvoker> jsCallInvoker) {
  auto pool = std::make_shared<ThreadPool>();
  invoker = jsCallInvoker;
  auto spawnTask = jsi::Function::createFromHostFunction(runtime,
                                                         jsi::PropNameID::forAscii(runtime, "spawnTask"),
                                                         1,
                                                         [pool, jsCallInvoker](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
    if (!arguments[0].isObject())
      throw jsi::JSError(runtime, "spawnTask: First argument has to be a function!");
    
    auto userCallbackRef = std::make_shared<jsi::Object>(arguments[0].getObject(runtime));
    
    if (!userCallbackRef->isFunction(runtime)) {
      throw jsi::JSError(runtime, "spawnTask: First argument has to be a function!");
    }
    
    auto task = [&runtime, jsCallInvoker, userCallbackRef = move(userCallbackRef)](){
      
      try {
        
        jsCallInvoker->invokeAsync([&runtime, userCallbackRef = move(userCallbackRef)]{
          userCallbackRef->asFunction(runtime).call(runtime);
        });
      } catch (jsi::JSError& error) {
        jsCallInvoker->invokeAsync([&runtime, &error] {
          jsi::JSError(runtime, error.what());
        });
      }
      
    };
    
    pool->queueWork(task);
    return {};
  });
  
  auto spawnTaskAsync = jsi::Function::createFromHostFunction(runtime,
                                                              jsi::PropNameID::forAscii(runtime, "spawnTaskAsync"),
                                                              1,
                                                              [pool, jsCallInvoker](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
    if (!arguments[0].isObject())
      throw jsi::JSError(runtime, "spawnTask: First argument has to be a function!");
    
    auto userCallbackRef = std::make_shared<jsi::Object>(arguments[0].getObject(runtime));
    
    if (!userCallbackRef->isFunction(runtime)) {
      throw jsi::JSError(runtime, "spawnTask: First argument has to be a function!");
    }
    
    
    auto promiseCallback = jsi::Function::createFromHostFunction(runtime,
                                                                 jsi::PropNameID::forAscii(runtime, "promiseCallback"),
                                                                 1,
                                                                 [pool, jsCallInvoker, userCallbackRef](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* arguments, size_t count) -> jsi::Value {
      auto resolverValue = std::make_shared<jsi::Value>((arguments[0].asObject(runtime)));
      auto rejecterValue = std::make_shared<jsi::Value>((arguments[1].asObject(runtime)));
      
      auto task = [&runtime, &jsCallInvoker, &resolverValue, &rejecterValue, &userCallbackRef](){
        try {
          auto result = userCallbackRef->asFunction(runtime).call(runtime);
          //jsi::Value res(result);
          jsCallInvoker->invokeAsync([&runtime, &result, &resolverValue]{
            const jsi::Value& res = result;
            resolverValue->asObject(runtime).asFunction(runtime).call(runtime, result);
   
          });
        } catch (jsi::JSError& error) {
          jsCallInvoker->invokeAsync([&runtime, &error, rejecterValue] {
            rejecterValue->asObject(runtime).asFunction(runtime).call(runtime, jsi::JSError(runtime, error.what()).value());
            
          });
        }
      };
      
      pool->queueWork(task);
      return jsi::Value::undefined();
    });
    
    auto newPromise = runtime.global().getPropertyAsFunction(runtime, "Promise");
    auto promise = newPromise
      .callAsConstructor(runtime, promiseCallback);
    
    return promise;
  });
  
  runtime.global().setProperty(runtime, "spawnTask", std::move(spawnTask));
  runtime.global().setProperty(runtime, "spawnTaskAsync", std::move(spawnTaskAsync));
}
}
