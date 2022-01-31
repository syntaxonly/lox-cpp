#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <string>

namespace lox {
// clang-format off
enum class tokenKind : std::uint8_t {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,                               // parenthesis
    EQUAL, EQUAL_EQUAL, BANG, BANG_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, // comparison
    PLUS, MINUS, STAR, SLASH,                                                       // operators
    DOT, COMMA, SEMICOLON,                                                          // dot
    STRING, IDENTIFIER, NUMBER,                                                     // expressions
    AND, OR, TRUE, FALSE,                                                           // boolean
    IF, ELSE,                                                                       // condition
    FOR, WHILE,                                                                     // loop
    CLASS, SUPER, FUN, RETURN, VAR, PRINT, THIS,                                    // keywords
    NIL, END_OF_FILE
  // clang-format on
};

using literal_t = std::optional<std::variant<std::string, double>>;

struct Token {
  tokenKind m_kind;
  literal_t m_lexeme;
  std::size_t m_line;
};

}