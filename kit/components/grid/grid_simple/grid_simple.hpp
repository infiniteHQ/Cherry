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
            GridSimple(const Cherry::Identifier &id, const std::vector<std::function<void()>> &elements, const float &blockSize = 250.0f, const float &minBlockSize = 250.0f)
                : Component(id), m_Elements(elements)
            {
                SetProperty("blockSize", std::to_string(blockSize));
                SetProperty("minBlockSize", std::to_string(minBlockSize));
            }

            GridSimple(const Cherry::Identifier &id, const std::vector<std::shared_ptr<Cherry::Component>> &elements, const float &blockSize = 250.0f, const float &minBlockSize = 250.0f)
                : Component(id), m_Components(elements)
            {
                SetProperty("blockSize", std::to_string(blockSize));
                SetProperty("minBlockSize", std::to_string(minBlockSize));
            }

            void Render() override
            {
                float block_size = std::stof(GetProperty("blockSize"));
                float min_block_size = std::stof(GetProperty("minBlockSize"));

                ImVec2 avail = CherryGUI::GetContentRegionAvail();
                int columns = std::max(1, (int)(avail.x / block_size));
                float actual_block_size = std::max(min_block_size, avail.x / columns);

                int index = 0;

                if (!m_Elements.empty())
                {
                    for (const auto &element : m_Elements)
                    {
                        if (element)
                        {
                            if (index > 0 && index % columns == 0)
                                CherryGUI::NewLine();

                            CherryGUI::BeginGroup();

                            std::string id = GetIdentifier().string() + std::to_string(index);
                            CherryGUI::PushID(id.c_str());
                            CherryGUI::SetNextItemWidth(actual_block_size);

                            element();

                            CherryGUI::PopID();
                            CherryGUI::EndGroup();

                            if (index < m_Elements.size() - 1)
                            {
                               CherryGUI::SameLine();
                            }
                            index++;
                        }
                    }
                }
                else
                {
                    for (const auto &component : m_Components)
                    {
                        if (component)
                        {
                            if (index > 0 && index % columns == 0)
                                CherryGUI::NewLine();

                            CherryGUI::BeginGroup();
                            std::string id = GetIdentifier().string() + std::to_string(index);
                            CherryGUI::PushID(id.c_str());
                            CherryGUI::SetNextItemWidth(actual_block_size);

                            component->Render();

                            CherryGUI::PopID();
                            CherryGUI::EndGroup();
                            
                            if (index < m_Components.size() - 1)
                            {
                                CherryGUI::SameLine();
                            }
                            index++;
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
        inline std::shared_ptr<Component> GridSimple(const std::vector<std::function<void()>> &elements, const float &blockSize = 250.0f, const float &minBlockSize = 250.0f)
        {
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::GridSimple>(Components::GridSimple(anonymous_id, elements, blockSize, minBlockSize));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> GridSimple(const Cherry::Identifier &identifier, const std::vector<std::function<void()>> &elements, const float &blockSize = 250.0f, const float &minBlockSize = 250.0f)
        {
            switch(identifier.property())
            {
                case IdentifierProperty::Inline : 
                {
                    auto new_title = Application::CreateAnonymousComponent<Components::GridSimple>(Components::GridSimple(identifier, elements, blockSize, minBlockSize));
                    new_title->Render();
                    return new_title;
                    break;
                }
                case IdentifierProperty::CreateOnly : 
                {
                    break;
                }
                case IdentifierProperty::None : 
                {
                    break;
                }
                default:
                {
                    break;
                }
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
                auto new_button = Application::CreateComponent<Components::GridSimple>(Components::GridSimple(identifier, elements, blockSize, minBlockSize));
                new_button->Render();
                return new_button;
            }
            return existing_button;
        }

        inline std::shared_ptr<Component> GridSimple(const std::vector<std::shared_ptr<Cherry::Component>> &elements, const float &blockSize = 250.0f, const float &minBlockSize = 250.0f)
        {
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::GridSimple>(Components::GridSimple(anonymous_id, elements, blockSize, minBlockSize));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> GridSimple(const Cherry::Identifier &identifier, const std::vector<std::shared_ptr<Cherry::Component>> &elements, const float &blockSize = 250.0f, const float &minBlockSize = 250.0f)
        {
            switch(identifier.property())
            {
                case IdentifierProperty::Inline : 
                {
                    auto new_title = Application::CreateAnonymousComponent<Components::GridSimple>(Components::GridSimple(identifier, elements, blockSize, minBlockSize));
                    new_title->Render();
                    return new_title;
                    break;
                }
                case IdentifierProperty::CreateOnly : 
                {
                    break;
                }
                case IdentifierProperty::None : 
                {
                    break;
                }
                default:
                {
                    break;
                }
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
                auto new_button = Application::CreateComponent<Components::GridSimple>(Components::GridSimple(identifier, elements, blockSize, minBlockSize));
                new_button->Render();
                return new_button;
            }
            return existing_button;
        }
    }

}

#endif // CHERRY_KIT_GRID_SIMPLE
