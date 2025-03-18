#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// GridSimple
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_GRID_SIMPLE
#define CHERRY_KIT_GRID_SIMPLE

namespace Cherry
{
    namespace Components
    {
        class GridSimple : public Component
        {
        public:
            GridSimple(const Cherry::Identifier &id, const std::vector<std::function<void()>> &elements)
                : Component(id), m_Elements(elements)
            {
            }

            GridSimple(const Cherry::Identifier &id, const std::vector<std::shared_ptr<Cherry::Component>> &elements)
                : Component(id), m_Components(elements)
            {
            }

            void Render() override
            {
                if (!m_Elements.empty())
                {
                    for (const auto &element : m_Elements)
                    {
                        if (element)
                        {
                            element();
                        }
                    }
                }
                else
                {
                    for (const auto &component : m_Components)
                    {
                        if (component)
                        {
                            component->Render();
                        }
                    }
                }
            }

        private:
            std::vector<std::function<void()>> m_Elements;
            std::vector<std::shared_ptr<Cherry::Component>> m_Components;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> GridSimple(const std::vector<std::function<void()>> &elements)
        {
            auto title = Application::CreateAnonymousComponent<Components::GridSimple>(Components::GridSimple(Cherry::Identifier(""), elements));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> GridSimple(const Cherry::Identifier &identifier, const std::vector<std::function<void()>> &elements)
        {
            if (identifier.string() == "__inline")
            {
                auto new_button = std::make_shared<Components::GridSimple>(Components::GridSimple(identifier, elements));
                new_button->Render();
                return new_button;
            }

            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::GridSimple>(Components::GridSimple(identifier, elements));
                new_button->Render();
                return new_button;
            }
            return existing_button;
        }
        
        inline std::shared_ptr<Component> GridSimple(const std::vector<std::shared_ptr<Cherry::Component>> &elements)
        {
            auto title = Application::CreateAnonymousComponent<Components::GridSimple>(Components::GridSimple(Cherry::Identifier(""), elements));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> GridSimple(const Cherry::Identifier &identifier, const std::vector<std::shared_ptr<Cherry::Component>> &elements)
        {
            if (identifier.string() == "__inline")
            {
                auto new_button = std::make_shared<Components::GridSimple>(Components::GridSimple(identifier, elements));
                new_button->Render();
                return new_button;
            }

            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::GridSimple>(Components::GridSimple(identifier, elements));
                new_button->Render();
                return new_button;
            }
            return existing_button;
        }
    }

}

#endif // CHERRY_KIT_GRID_SIMPLE
