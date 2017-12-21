#include <functional>
#include <iostream>
#include <optional>
#include <reyco/ConsoleFactory.h>
#include <unordered_map>

namespace reyco {

class Console {
public:
  class CmdHandler;

  Console(std::istream &in, std::ostream &out) : in(in), out(out) {}
  bool start();

  auto addCommand(const std::string_view &cmd, auto handler);

private:
  Console &registerCmd(const std::string_view &cmd, CmdHandler &handler) {
    cmdHandlers.emplace(cmd, std::ref(handler));
    return *this;
  }
  void unregisterCmd(const std::string_view &cmd) { cmdHandlers.erase(cmd); }

  std::istream &in;
  std::ostream &out;
  using CmdHandlerRef = std::reference_wrapper<CmdHandler>;
  std::unordered_map<std::string_view, CmdHandlerRef> cmdHandlers;
};

class Console::CmdHandler {
public:
  CmdHandler(const std::string_view &cmd, Console &console)
      : cmd(cmd), console(console.registerCmd(cmd, *this)) {}
  virtual ~CmdHandler() { console.unregisterCmd(cmd); }

  virtual std::optional<bool> handle() = 0;

protected:
  std::string_view cmd;
  Console &console;
};

bool Console::start() {
  std::string cmd = "(started)";
  while (true) {
    out << '\n' << cmd << ">> ";
    if (!(in >> cmd))
      return false;

    try {
      CmdHandler &handler = cmdHandlers.at(cmd).get();
      std::optional<bool> result = handler.handle();

      if (!result.has_value())
        continue;
      return *result;
    } catch (std::out_of_range) {
      using namespace std::string_literals;
      throw std::runtime_error("no such command: '"s + cmd + "'"s);
    }
  }
  return false;
}

auto Console::addCommand(const std::string_view &cmd, auto handler) {

  constexpr bool isVoid = std::is_void_v<decltype(handler(in, out))>;
  constexpr auto innerHandler = handler;
  if
    constexpr(isVoid) {
      class InlineCmdHandler : public Console::CmdHandler {
      public:
        using CmdHandler::CmdHandler;
        std::optional<bool> handle() override {
          innerHandler(console.in, console.out);
          return {};
        }
      };
      return std::make_unique<InlineCmdHandler>(cmd, *this);
    }
  else {
    constexpr auto recipe = handler;
    class InlineCmdHandler : public Console::CmdHandler {
    public:
      using CmdHandler::CmdHandler;
      std::optional<bool> handle() override {
        return innerHandler(console.in, console.out);
      }
    };
    return std::make_unique<InlineCmdHandler>(cmd, *this);
  }
}

ConsoleFactory::~ConsoleFactory() {}

ConsoleFactory ConsoleFactory::make(std::istream &in, std::ostream &out) {
  auto console = std::make_unique<Console>(in, out);
  return ConsoleFactory{std::move(console)};
}

bool ConsoleFactory::start() const {
  Console &console = *get();
  auto nop = console.addCommand("nop", [](auto &, auto &) {});
  auto exit = console.addCommand("exit", [](auto &, auto &) { return false; });
  auto r = console.addCommand("restart", [](auto &, auto &) { return true; });
  return console.start();
}
}
