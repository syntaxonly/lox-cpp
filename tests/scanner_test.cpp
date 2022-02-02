#include "lox/token.h"
#include "lox/scanner/scanner.h"

#include <magic_enum.hpp>

#include <fstream>
#include <iterator>
#include <string>
#include <iostream>

int main() {
  std::ifstream fin;

  fin.open("scanner.in");
  const auto source = std::string(std::istream_iterator<char>(fin >> std::noskipws), {});

  lox::Scanner scanner(source);
  const auto tokens = scanner.scan();

  if (tokens.empty())
    return 1;

  for (auto token : tokens) {
    std::cout << token.line << '\t';
    std::cout << magic_enum::enum_name(token.kind) << ' ';

    if (token.lexeme.has_value()) {
      if (auto ret = std::get_if<std::string>(&token.lexeme.value()))
        std::cout << *ret << ' ';
      else if (auto ret = std::get_if<double>(&token.lexeme.value()))
        std::cout << *ret << ' ';
    }

    std::cout << '\n';
  }

  return 0;
}