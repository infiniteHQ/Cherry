#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"
#include "../../../../platform/engine/cef.hpp"

//
// WebView
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_WEB_VIEW
#define CHERRY_KIT_WEB_VIEW

namespace Cherry
{
    namespace Components
    {
        class WebView : public Component
        {
        public:
            WebView(const Cherry::Identifier &id, const std::string &url)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                SetProperty("url", url);

            }

            void Render() override
            {
                const ImVec2 &size = ImVec2(500, 500);

                CherryGUI::Text(GetProperty("url").c_str());
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> WebView(const std::string &url)
        {
            auto anonymous_id = Application::GetAnonymousID(url);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto title = Application::CreateAnonymousComponent<Components::WebView>(Components::WebView(anonymous_id, url));
                title->Render();
                return title;
            }
        }

        inline std::shared_ptr<Component> WebView(const Cherry::Identifier &identifier, const std::string &url)
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = std::make_shared<Components::WebView>(Components::WebView(identifier, url));
                new_title->Render();
                return new_title;
            }

            // Get the object if exist
            auto existing_title = Application::GetComponent(identifier);
            if (existing_title)
            {
                existing_title->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_title = Application::CreateComponent<Components::WebView>(Components::WebView(identifier, url));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_WEB_VIEW
