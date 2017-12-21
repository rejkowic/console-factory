#pragma once
#include <memory>

namespace reyco {

class Console;

struct ConsoleFactory : std::unique_ptr<Console> {
  ~ConsoleFactory();
  static ConsoleFactory make(std::istream &in, std::ostream &out);
  bool start() const;
};
}
