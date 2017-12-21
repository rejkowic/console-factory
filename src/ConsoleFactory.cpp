#include <iostream>
#include <reyco/ConsoleFactory.h>

namespace reyco {

class Console {
public:
  Console(std::istream &in, std::ostream &out) : in(in), out(out) {}
  bool start() {
    std::string cmd = "restart";
    while (true) {
      out << '\n' << cmd << ">> ";
      if (!(in >> cmd))
        return false;
      if (!cmd.compare("exit"))
        return false;
      if (!cmd.compare("restart"))
        return true;

      if (!cmd.compare("nop"))
        continue;

      using namespace std::string_literals;
      throw std::runtime_error("no such command: '"s + cmd + "'"s);
    }
    return false;
  }

private:
  std::istream &in;
  std::ostream &out;
};

ConsoleFactory::~ConsoleFactory() {}

ConsoleFactory ConsoleFactory::make(std::istream &in, std::ostream &out) {
  auto console = std::make_unique<Console>(in, out);
  return ConsoleFactory{std::move(console)};
}

bool ConsoleFactory::start() const { return get()->start(); }
}
