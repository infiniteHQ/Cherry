#pragma once

#include <mutex>
#include <random>

#include "../base.hpp"

#ifndef CHERRY_IDENTIFIER
#define CHERRY_IDENTIFIER

namespace Cherry {

class Component;
struct ComponentsPool {
  std::vector<std::shared_ptr<Component>> AnonymousComponents;
  std::vector<std::shared_ptr<Component>> IdentifiedComponents;
};

class IdentifierPattern {
public:
  template <typename... Args>
  IdentifierPattern(RenderMode property, Args &&...args) : prop(property) {
    (parameters.emplace_back(std::forward<Args>(args)), ...);
  }

  RenderMode GetProperty() const { return prop; }

  const std::vector<std::any> &GetParameters() const { return parameters; }

private:
  RenderMode prop;
  std::vector<std::any> parameters;
};

class Identifier {
public:
  Identifier();
  Identifier(const std::string &id, RenderMode property = RenderMode::None);

  explicit Identifier(ComponentsPool *components_, const std::string &id = "",
                      RenderMode property = RenderMode::None);
  Identifier(const Identifier &other) = default;
  Identifier &operator=(const Identifier &other) = default;

  [[nodiscard]] RenderMode property() const { return m_RenderMode; }
  [[nodiscard]] const std::string &string() const { return m_IdentifierName; }
  ComponentsPool *component_array_ptr() const { return m_ComponentArrayPtr; }
  [[nodiscard]] const std::string &component_group() const {
    return m_ComponentGroup;
  }

  void set_component_group(const std::string &group) {
    m_ComponentGroup = group;
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

private:
  std::string m_IdentifierName;
  std::string m_ComponentGroup;
  ComponentsPool *m_ComponentArrayPtr = nullptr;

  RenderMode m_RenderMode;

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
inline IdentifierPattern ID(RenderMode prop) { return IdentifierPattern(prop); }

template <typename... Args>
inline IdentifierPattern ID(RenderMode prop, Args &&...args) {
  return IdentifierPattern(prop, std::forward<Args>(args)...);
}

} // namespace Cherry

// TODO : Macro to allow this
using CherryID = Cherry::Identifier;
using RenderMode = Cherry::RenderMode;
using ComponentsPool = Cherry::ComponentsPool;

#endif // CHERRY_IDENTIFIER
