#include "identifier.hpp"
#include "app.hpp"

namespace Cherry {

Identifier::Identifier()
    : m_IdentifierName("undefined"), m_ComponentArrayPtr(nullptr),
      m_RenderMode(RenderMode::None) {
  if (m_IdentifierName.empty()) {
    m_IdentifierName = GetUniqueIndex();
  }
}

Identifier::Identifier(const std::string &id, RenderMode property)
    : m_IdentifierName(id), m_RenderMode(property) {
  m_ComponentGroup = CherryApp.GetComponentGroup();
  if (m_IdentifierName.empty()) {
    m_IdentifierName = "undefined";
  }
}

Identifier::Identifier(ComponentsPool *components_, const std::string &id,
                       RenderMode property)
    : m_IdentifierName(id), m_ComponentArrayPtr(components_),
      m_RenderMode(property) {
  m_ComponentGroup = CherryApp.GetComponentGroup();
  if (m_IdentifierName.empty()) {
    m_IdentifierName = GetUniqueIndex();
  }
}

} // namespace Cherry