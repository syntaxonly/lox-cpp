#include "lox/interpreter/interpreter.h"
#include "lox/token.h"
#include "lox/ast/ast.h"

#include <boost/variant/static_visitor.hpp>
#include <variant>
#include <string>
#include <iostream>

namespace lox {

using Values = std::variant<std::nullptr_t, bool, double, std::string>;

std::string to_string(const Values &values) {
  // clang-format off
  using namespace std::string_literals;
  return std::visit(overload {
           [](const std::nullptr_t) { return "null"s; },
           [](const bool b) { return b ? "true"s: "false"s; },
           [](const double d) { return std::to_string(d); },
           [](const std::string& s) { return s;}
      }, values);
  // clang-format on
}

bool isTruthy(const Values &values) {
  // clang-format off
  return std::visit(overload {
           [](const std::nullptr_t) { return false; },
           [](const bool ) { return true; },
           [](const double ) { return true; },
           [](const std::string &) { return true; }
      }, values);
  // clang-format on
}

bool operator==(const Values &left, const Values &right) {
  if (left.index() == 0 && right.index() == 0)
    return true;
  if (left.index() == 0 && right.index() != 0)
    return false;
  return left == right;
}

bool operator!=(const Values &left, const Values &right) {
  return !(left == right);
}

struct ExpressionVisitor : public boost::static_visitor<Values> {
  Values operator()(const LiteralExpr &expr) const {
    return to_string(expr.literal);
  }

  Values operator()(const GroupingExpr &expr) const {
    return boost::apply_visitor(ExpressionVisitor(), expr.expression);
  }

  Values operator()(const UnaryExpr &expr) const {
    Values right = boost::apply_visitor(ExpressionVisitor(), expr.right);

    switch (expr.op.kind) {
      using enum TokenKind;

      case MINUS:
        return -std::get<double>(right);

      case BANG:
        return isTruthy(right);

      default:;
    }

    return nullptr;
  }

  Values operator()(const BinaryExpr &expr) const {
    Values left = boost::apply_visitor(ExpressionVisitor(), expr.left);
    Values right = boost::apply_visitor(ExpressionVisitor(), expr.right);

    switch (expr.op.kind) {
      using enum TokenKind;

      case PLUS: {
        if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
          return std::get<double>(left) + std::get<double>(right);

        if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
          return std::get<std::string>(left) + std::get<std::string>(right);
      }

      case MINUS:
        return std::get<double>(left) - std::get<double>(right);

      case SLASH:
        return std::get<double>(left) / std::get<double>(right);

      case STAR:
        return std::get<double>(left) * std::get<double>(right);

      case GREATER:
        return std::get<double>(left) > std::get<double>(right);

      case GREATER_EQUAL:
        return std::get<double>(left) >= std::get<double>(right);

      case LESS:
        return std::get<double>(left) < std::get<double>(right);

      case LESS_EQUAL:
        return std::get<double>(left) <= std::get<double>(right);

      case EQUAL_EQUAL:
        return left == right;

      case BANG_EQUAL:
        return left != right;

      default:
        break;
    }

    return nullptr;
  }

  Values operator()([[maybe_unused]] const auto & /*unused*/) const {
    return {};
  }
};

struct StatementVisitor : public boost::static_visitor<void> {
  void operator()(const ExpressionStmt &stmt) const {
    boost::apply_visitor(ExpressionVisitor(), stmt.expression);
  }

  void operator()(const PrintStmt &stmt) const {
    Values val = boost::apply_visitor(ExpressionVisitor(), stmt.expression);
    std::cout << to_string(val);
  }

  void operator()([[maybe_unused]] const auto & /*unused*/) const {
  }
};

Interpreter::Interpreter(const std::vector<Stmt> &statements)
    : statements(statements) {
}

void Interpreter::interpret() const {
  try {
    for (const auto &statement : statements) {
      boost::apply_visitor(StatementVisitor(), statement);
    }
  } catch (const std::runtime_error &e) {
    throw e.what();
  }
}

}
