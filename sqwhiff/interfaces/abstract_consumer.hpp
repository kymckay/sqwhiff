#pragma once

namespace sqwhiff {

template <class T>
class AbstractConsumer {
 public:
  virtual ~AbstractConsumer(){};
  virtual T advance() = 0;
  virtual T peek() = 0;
  virtual T at() = 0;
};

}  // namespace sqwhiff
