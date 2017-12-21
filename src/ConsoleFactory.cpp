#include <iostream>
#include <reyco/ConsoleCmdHandler.h>
#include <reyco/ConsoleFactory.h>

namespace reyco {

ConsoleFactory::~ConsoleFactory() {}

ConsoleFactory ConsoleFactory::make(std::istream &in, std::ostream &out) {
  auto console = Console::make(in, out);
  return ConsoleFactory{std::move(console)};
}

bool ConsoleFactory::start() const {
  Console &console = *get();
  auto nop =
      console.addCommand("nop", [](auto &, auto &out) { out << "nop\n"; });
  auto exit = console.addCommand("exit", [](auto &, auto &) { return false; });
  auto r = console.addCommand("restart", [](auto &, auto &) { return true; });
  return console.start();
}
}
