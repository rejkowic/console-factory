#include <fstream>
#include <iostream>
#include <reyco/AppFactory.h>
#include <reyco/ConsoleFactory.h>

namespace reyco {

static std::ifstream fromFile{"in.txt"};
bool consoleTest(const AppFactory::Argv &argv) {
  std::istream &in = argv.size() > 1 ? std::cin : fromFile;
  ConsoleFactory console = ConsoleFactory::make(in, std::cout);
  return console.start();
}
}

int main(int argc, char *argv[]) {
  return reyco::AppFactory::make(reyco::consoleTest, argc, argv);
}
