//
//  identifier.hpp
//  Headers for Cherry identifiers (for components ID and other identifications needs)
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <main/engine/base.hpp>
#include <main/engine/options.hpp>
#include <mutex>
#include <random>

#ifndef CHERRY_IDENTIFIER
#define CHERRY_IDENTIFIER

namespace Cherry {

  class Component;
  struct ComponentsPool {
    std::vector<std::shared_ptr<Component>> AnonymousComponents;
    std::vector<std::shared_ptr<Component>> IdentifiedComponents;
  };

  class CHERRY_API IdentifierPattern {
   public:
    template<typename... Args>
    IdentifierPattern(RenderMode property, Args &&...args) : prop(property) {
      (parameters.emplace_back(std::forward<Args>(args)), ...);
    }

    const RenderMode GetProperty() const;
    const std::vector<std::any> &GetParameters() const;

   private:
    RenderMode prop;
    std::vector<std::any> parameters;
  };

  class CHERRY_API Identifier {
   public:
    Identifier();
    Identifier(const std::string &id, RenderMode property = RenderMode::None);

    explicit Identifier(ComponentsPool *components_, const std::string &id = "", RenderMode property = RenderMode::None);
    Identifier(const Identifier &other) = default;
    Identifier &operator=(const Identifier &other) = default;

    ComponentsPool *component_array_ptr() const;
    [[nodiscard]] const RenderMode &property() const;
    [[nodiscard]] const std::string &string() const;
    [[nodiscard]] const std::string &component_group() const;

    void set_component_group(const std::string &group);
    void set(const std::string &id);
    bool operator==(const Identifier &other) const;

    static void UpgradeIncrementorLevel();
    static void DowngradeIncrementorLevel();
    static std::string GetUniqueIndex();
    static std::string GetRandomIndex();
    static void ResetUniqueIndex();

   private:
    std::string m_IdentifierName;
    std::string m_ComponentGroup;
    ComponentsPool *m_ComponentArrayPtr = nullptr;

    RenderMode m_RenderMode;

    static inline int incrementor_level = 1;
    static inline std::vector<int> level_indices = { 0 };
    static inline std::mutex mutex_;

    static std::string generate_random_id();
    static std::string generate_id();
    static std::string convert_to_letters(int index);
  };

  // Some API functions
  inline IdentifierPattern ID(RenderMode prop) {
    return IdentifierPattern(prop);
  }

  template<typename... Args>
  inline IdentifierPattern ID(RenderMode prop, Args &&...args) {
    return IdentifierPattern(prop, std::forward<Args>(args)...);
  }

}  // namespace Cherry

// TODO : Macro to allow this
using CherryID = Cherry::Identifier;
using RenderMode = Cherry::RenderMode;
using ComponentsPool = Cherry::ComponentsPool;

#endif  // CHERRY_IDENTIFIER
