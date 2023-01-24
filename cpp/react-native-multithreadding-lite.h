#ifndef MULTITHREADDINGLITE_H
#define MULTITHREADDINGLITE_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>
#include <ReactCommon/CallInvoker.h>

using namespace facebook;


namespace multithreaddinglite
{
  double multiply(double a, double b);
  void install(jsi::Runtime &rt, std::shared_ptr<react::CallInvoker> jsCallInvoker);
}

#endif /* MULTITHREADDINGLITE_H */
