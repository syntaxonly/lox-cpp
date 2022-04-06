#include "lox/callable/instance.h"
#include "lox/token.h"
#include "lox/error/error.h"

#include <string>

namespace lox {

Instance::Instance(const Class &klass)
    : m_klass(klass){};

std::any Instance::get(const Token &name) {
  if (m_fields.contains(name.lexeme)) {
    return m_fields.find(name.lexeme)->second;
  }

  throw RuntimeError(name, std::string("Undefined property '").append(name.lexeme).append("'."));
}

Instance::operator std::string() const {
  return std::string(m_klass).append(" instance");
}

}
