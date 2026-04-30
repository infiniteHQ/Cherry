//
//  identifier.hpp
//  Sources for Cherry identifiers (for components ID and other identifications needs)
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "identifier.hpp"

#include "../app/app.hpp"

namespace Cherry {

  Identifier::Identifier() : m_IdentifierName("undefined"), m_ComponentArrayPtr(nullptr), m_RenderMode(RenderMode::None) {
    if (m_IdentifierName.empty()) {
      m_IdentifierName = GetUniqueIndex();
    }
  }

  Identifier::Identifier(const std::string &id, RenderMode property) : m_IdentifierName(id), m_RenderMode(property) {
    m_ComponentGroup = CherryApp.GetComponentGroup();
    if (m_IdentifierName.empty()) {
      m_IdentifierName = "undefined";
    }
  }

  Identifier::Identifier(ComponentsPool *components_, const std::string &id, RenderMode property)
      : m_IdentifierName(id),
        m_ComponentArrayPtr(components_),
        m_RenderMode(property) {
    m_ComponentGroup = CherryApp.GetComponentGroup();
    if (m_IdentifierName.empty()) {
      m_IdentifierName = GetUniqueIndex();
    }
  }

  ComponentsPool *Identifier::component_array_ptr() const {
    return m_ComponentArrayPtr;
  }

  [[nodiscard]] const RenderMode &Identifier::property() const {
    return m_RenderMode;
  }

  [[nodiscard]] const std::string &Identifier::string() const {
    return m_IdentifierName;
  }

  [[nodiscard]] const std::string &Identifier::component_group() const {
    return m_ComponentGroup;
  }

  void Identifier::set_component_group(const std::string &group) {
    m_ComponentGroup = group;
  }

  void Identifier::set(const std::string &id) {
    m_IdentifierName = id;
  }

  bool Identifier::operator==(const Identifier &other) const {
    return m_IdentifierName == other.m_IdentifierName;
  }

  void Identifier::UpgradeIncrementorLevel() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (++incrementor_level > static_cast<int>(level_indices.size()))
      level_indices.push_back(0);
  }

  void Identifier::DowngradeIncrementorLevel() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (incrementor_level > 1)
      --incrementor_level;
  }

  std::string Identifier::GetUniqueIndex() {
    std::lock_guard<std::mutex> lock(mutex_);
    int level = incrementor_level - 1;
    if (level >= static_cast<int>(level_indices.size()))
      level_indices.push_back(0);

    ++level_indices[level];
    return generate_id();
  }

  std::string Identifier::GetRandomIndex() {
    std::lock_guard<std::mutex> lock(mutex_);
    return generate_random_id();
  }

  void Identifier::ResetUniqueIndex() {
    std::lock_guard<std::mutex> lock(mutex_);
    incrementor_level = 1;
    level_indices = { 0 };
  }

  std::string Identifier::generate_random_id() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<uint64_t> distribution(1, UINT64_MAX);

    return "RAND_" + std::to_string(distribution(generator));
  }

  std::string Identifier::generate_id() {
    std::string id;
    int level = incrementor_level;

    for (size_t i = 0; i < level_indices.size() && i < static_cast<size_t>(level); ++i) {
      id += std::to_string(level_indices[i]);
      if (i % 2 == 1)
        id += convert_to_letters(level_indices[i]);
    }
    return id;
  }

  std::string Identifier::convert_to_letters(int index) {
    std::string result;
    while (index >= 0) {
      result.insert(result.begin(), 'a' + (index % 26));
      index = (index / 26) - 1;
    }
    return result;
  }

  const RenderMode IdentifierPattern::GetProperty() const {
    return prop;
  }

  const std::vector<std::any> &IdentifierPattern::GetParameters() const {
    return parameters;
  }

}  // namespace Cherry