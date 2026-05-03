//
//  factory.hpp
//  Headers for Cherry component factory
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once
#include <any>
#include <cstdio>
#include <functional>
#include <main/core/assert/assert.hpp>
#include <main/engine/app/app.hpp>
#include <main/engine/components/components.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace Cherry {

  using Props = std::unordered_map<std::string, std::any>;
  using RenderFn = std::function<void()>;

  using FactoryRef = std::function<Component&(const Identifier&, const Props&)>;
  using FactoryOwned = std::function<std::unique_ptr<Component>(const Identifier&, const Props&)>;

  template<typename T>
  T PropOr(const Props& props, const std::string& key, T defaultVal) {
    auto it = props.find(key);
    if (it == props.end())
      return defaultVal;
    return std::any_cast<T>(it->second);
  }

  class ComponentDefaultsRegistry {
   public:
    static ComponentDefaultsRegistry& Instance() {
      static ComponentDefaultsRegistry inst;
      return inst;
    }
    void SetOnce(const std::string& name, Props defaults) {
      if (m_defaults.count(name))
        return;
      m_defaults[name] = std::move(defaults);
    }
    const Props* Get(const std::string& name) const {
      auto it = m_defaults.find(name);
      return it != m_defaults.end() ? &it->second : nullptr;
    }

   private:
    std::unordered_map<std::string, Props> m_defaults;
  };

  namespace detail {
    inline std::string AnyToString(const std::any& val) {
      if (val.type() == typeid(std::string))
        return std::any_cast<std::string>(val);
      if (val.type() == typeid(const char*))
        return std::string(std::any_cast<const char*>(val));
      if (val.type() == typeid(int))
        return std::to_string(std::any_cast<int>(val));
      if (val.type() == typeid(float))
        return std::to_string(std::any_cast<float>(val));
      if (val.type() == typeid(bool))
        return std::any_cast<bool>(val) ? "true" : "false";
      return "";
    }

    inline void ApplyPropsMap(Component& comp, const Props& props) {
      for (const auto& [key, val] : props) {
        if (key.rfind("prop:", 0) == 0)
          comp.SetProperty(key.substr(5), AnyToString(val));
        else if (key.rfind("data:", 0) == 0)
          comp.SetData(key.substr(5), AnyToString(val));
      }
    }

    inline void ApplyWithDefaults(Component& comp, const Props& callProps, const std::string& name) {
      if (const Props* defs = ComponentDefaultsRegistry::Instance().Get(name))
        ApplyPropsMap(comp, *defs);
      ApplyPropsMap(comp, callProps);
    }

    inline std::unordered_set<std::string>& UnknownLogged() {
      static std::unordered_set<std::string> s;
      return s;
    }

    inline void WarnUnknown(const std::string& name) {
      if (UnknownLogged().count(name))
        return;
      UnknownLogged().insert(name);
      std::fprintf(stderr, "[Cherry] Component \"%s\" doesn't exist!\n", name.c_str());
      std::fflush(stderr);
    }
  }  // namespace detail

  namespace Components {
    class DynamicComponent : public Component {
     public:
      DynamicComponent(const Identifier& id, std::shared_ptr<RenderFn> fn) : Component(id), m_fn(std::move(fn)) {
        SetIdentifier(id);
      }
      void Render() override {
        if (!m_fn || !*m_fn)
          return;
        (*m_fn)();
      }

     private:
      std::shared_ptr<RenderFn> m_fn;
    };

    class UnknownComponent : public Component {
     public:
      UnknownComponent(const Identifier& id, const std::string& name) : Component(id), m_name(name) {
        SetIdentifier(id);
      }
      void Render() override {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.3f, 0.3f, 1.f));
        ImGui::Text("[Cherry] Unknown component: \"%s\"", m_name.c_str());
        ImGui::PopStyleColor();
      }

     private:
      std::string m_name;
    };
  }  // namespace Components

  class ComponentRegistry {
   public:
    static ComponentRegistry& Instance() {
      static ComponentRegistry inst;
      return inst;
    }

    void RegisterRef(const std::string& name, FactoryRef f) {
      m_ref[name] = std::move(f);
    }
    void RegisterOwned(const std::string& name, FactoryOwned f) {
      m_owned[name] = std::move(f);
    }

    const FactoryRef* FindRef(const std::string& n) const {
      auto it = m_ref.find(n);
      return it != m_ref.end() ? &it->second : nullptr;
    }
    const FactoryOwned* FindOwned(const std::string& n) const {
      auto it = m_owned.find(n);
      return it != m_owned.end() ? &it->second : nullptr;
    }
    bool Exists(const std::string& n) const {
      return m_ref.count(n) || m_owned.count(n);
    }

   private:
    std::unordered_map<std::string, FactoryRef> m_ref;
    std::unordered_map<std::string, FactoryOwned> m_owned;
  };

  class ComponentCache {
   public:
    static ComponentCache& Instance() {
      static ComponentCache inst;
      return inst;
    }

    Component* Get(const std::string& id) {
      auto it = m_cache.find(id);
      if (it == m_cache.end())
        return nullptr;
      it->second.lastSeen = m_frame;
      return it->second.ptr;
    }

    Component& StoreOwned(const std::string& id, std::unique_ptr<Component> c) {
      Component* raw = c.get();
      m_cache[id] = { raw, m_frame, std::move(c) };
      return *raw;
    }

    void StoreRef(const std::string& id, Component* raw) {
      m_cache[id] = { raw, m_frame, nullptr };
    }

    void EndFrame() {
      ++m_frame;
      m_instanceCounters.clear();
      constexpr uint64_t kKeepFrames = 2;
      for (auto it = m_cache.begin(); it != m_cache.end();) {
        if (m_frame - it->second.lastSeen > kKeepFrames)
          it = m_cache.erase(it);
        else
          ++it;
      }
    }

    std::string AllocId(const std::string& name) {
      return name + "##" + std::to_string(m_instanceCounters[name]++);
    }

   private:
    struct Entry {
      Component* ptr = nullptr;
      uint64_t lastSeen = 0;
      std::unique_ptr<Component> owned;
    };
    std::unordered_map<std::string, Entry> m_cache;
    std::unordered_map<std::string, uint32_t> m_instanceCounters;
    uint64_t m_frame = 0;
  };

  inline void Use(const std::string& name, const Props& defaults, RenderFn fn) {
    auto& registry = ComponentRegistry::Instance();
    if (!registry.Exists(name)) {
      auto sharedFn = std::make_shared<RenderFn>(std::move(fn));
      registry.RegisterOwned(name, [sharedFn](const Identifier& id, const Props&) -> std::unique_ptr<Component> {
        return std::make_unique<Components::DynamicComponent>(id, sharedFn);
      });
    }
    ComponentDefaultsRegistry::Instance().SetOnce(name, defaults);
  }

  inline void Use(const std::string& name, RenderFn fn) {
    Use(name, {}, std::move(fn));
  }

  inline void SetComponentDefaults(const std::string& name, Props defaults) {
    ComponentDefaultsRegistry::Instance().SetOnce(name, std::move(defaults));
  }

  inline Component& Draw(const std::string& componentName, const Props& props = {}) {
    auto& cache = ComponentCache::Instance();
    auto& registry = ComponentRegistry::Instance();

    std::string id;
    if (auto it = props.find("id"); it != props.end())
      id = componentName + "##" + std::any_cast<std::string>(it->second);
    else
      id = cache.AllocId(componentName);

    if (Component* cached = cache.Get(id)) {
      detail::ApplyPropsMap(*cached, props);
      cached->RenderWrapper();
      return *cached;
    }

    // already owned
    if (const FactoryOwned* f = registry.FindOwned(componentName)) {
      auto uptr = (*f)(Identifier(id), props);
      detail::ApplyWithDefaults(*uptr, props, componentName);
      Component& ref = cache.StoreOwned(id, std::move(uptr));
      ref.RenderWrapper();
      return ref;
    }

    // ref
    if (const FactoryRef* f = registry.FindRef(componentName)) {
      Component& ref = (*f)(Identifier(id), props);
      detail::ApplyWithDefaults(ref, props, componentName);
      cache.StoreRef(id, &ref);
      ref.RenderWrapper();
      return ref;
    }

    // if unknown
    detail::WarnUnknown(componentName);
    auto unk = std::make_unique<Components::UnknownComponent>(Identifier(id), componentName);
    return cache.StoreOwned(id, std::move(unk));
  }

}  // namespace Cherry

#define CHERRY_REGISTER_COMPONENT(ComponentName, FactoryBody)                                                    \
  namespace {                                                                                                    \
    struct _CherryAutoReg_##ComponentName {                                                                      \
      _CherryAutoReg_##ComponentName() {                                                                         \
        Cherry::ComponentRegistry::Instance().RegisterRef(                                                       \
            #ComponentName,                                                                                      \
            [](const Cherry::Identifier& id, const Cherry::Props& props) -> Cherry::Component& { FactoryBody }); \
      }                                                                                                          \
    } _g_cherryAutoReg_##ComponentName;                                                                          \
  }