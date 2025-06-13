#pragma once

#include <mutex>
#include <random>

#include "base.hpp"

#ifndef CHERRY_IDENTIFIER
#define CHERRY_IDENTIFIER

namespace Cherry {
class Component;

enum class IdentifierProperty {
  CreateOnly,
  Inline,
  None,
};

class IdentifierPattern {
public:
  template <typename... Args>
  IdentifierPattern(IdentifierProperty property, Args &&...args)
      : prop(property) {
    (parameters.emplace_back(std::forward<Args>(args)), ...);
  }

  IdentifierProperty GetProperty() const { return prop; }

  const std::vector<std::any> &GetParameters() const { return parameters; }

private:
  IdentifierProperty prop;
  std::vector<std::any> parameters;
};

class Identifier {
public:
  Identifier()
      : m_IdentifierName("undefined"), m_ComponentArrayPtr(nullptr),
        m_IdentifierProperty(IdentifierProperty::None) {
    if (m_IdentifierName.empty()) {
      m_IdentifierName = GetUniqueIndex();
    }
  }

  Identifier(const std::string &id,
             IdentifierProperty property = IdentifierProperty::None)
      : m_IdentifierName(id), m_IdentifierProperty(property) {
    if (m_IdentifierName.empty()) {
      m_IdentifierName = "undefined";
    }
  }

  explicit Identifier(std::vector<std::shared_ptr<Component>> *components_,
                      const std::string &id = "",
                      IdentifierProperty property = IdentifierProperty::None)
      : m_IdentifierName(id), m_ComponentArrayPtr(components_),
        m_IdentifierProperty(property) {
    if (m_IdentifierName.empty()) {
      m_IdentifierName = GetUniqueIndex();
    }
  }

  Identifier(const Identifier &other) = default;
  Identifier &operator=(const Identifier &other) = default;

  [[nodiscard]] IdentifierProperty property() const {
    return m_IdentifierProperty;
  }
  [[nodiscard]] const std::string &string() const { return m_IdentifierName; }
  [[nodiscard]] std::vector<std::shared_ptr<Component>> *
  component_array_ptr() const {
    return m_ComponentArrayPtr;
  }
  [[nodiscard]] const std::string &component_group() const {
    return m_ComponentGroup;
  }

  void set(const std::string &id) { m_IdentifierName = id; }
  bool operator==(const Identifier &other) const {
    return m_IdentifierName == other.m_IdentifierName;
  }

  static void UpgradeIncrementorLevel() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (++incrementor_level > static_cast<int>(level_indices.size()))
      level_indices.push_back(0);
  }

  static void DowngradeIncrementorLevel() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (incrementor_level > 1)
      --incrementor_level;
  }

  static std::string GetUniqueIndex() {
    std::lock_guard<std::mutex> lock(mutex_);
    int level = incrementor_level - 1;
    if (level >= static_cast<int>(level_indices.size()))
      level_indices.push_back(0);

    ++level_indices[level];
    return generate_id();
  }

  static std::string GetRandomIndex() {
    std::lock_guard<std::mutex> lock(mutex_);
    return generate_random_id();
  }

  static void ResetUniqueIndex() {
    std::lock_guard<std::mutex> lock(mutex_);
    incrementor_level = 1;
    level_indices = {0};
  }

  static std::shared_ptr<Component> GetComponent() {
    return nullptr; // TODO
  }

  std::string m_ComponentGroup;

private:
  std::string m_IdentifierName;
  std::vector<std::shared_ptr<Component>> *m_ComponentArrayPtr = nullptr;

  IdentifierProperty m_IdentifierProperty;

  static inline int incrementor_level = 1;
  static inline std::vector<int> level_indices = {0};
  static inline std::mutex mutex_;

  static std::string generate_random_id() {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<uint64_t> distribution(1, UINT64_MAX);

    return "RAND_" + std::to_string(distribution(generator));
  }

  static std::string generate_id() {
    std::string id;
    int level = incrementor_level;

    for (size_t i = 0;
         i < level_indices.size() && i < static_cast<size_t>(level); ++i) {
      id += std::to_string(level_indices[i]);
      if (i % 2 == 1)
        id += convert_to_letters(level_indices[i]);
    }
    return id;
  }

  static std::string convert_to_letters(int index) {
    std::string result;
    while (index >= 0) {
      result.insert(result.begin(), 'a' + (index % 26));
      index = (index / 26) - 1;
    }
    return result;
  }
};

// Some API functions
inline IdentifierPattern ID(IdentifierProperty prop) {
  return IdentifierPattern(prop);
}

template <typename... Args>
inline IdentifierPattern ID(IdentifierProperty prop, Args &&...args) {
  return IdentifierPattern(prop, std::forward<Args>(args)...);
}

} // namespace Cherry

using CherryID = Cherry::Identifier;

#endif // CHERRY_IDENTIFIER
