#include "app_window.hpp"
#include "app.hpp"

#include "../../src/core/log.hpp"

namespace Cherry
{

    AppWindow::AppWindow() : m_ID("undefined"), m_Name("undefined")
    {
        //
    }

    AppWindow::AppWindow(const std::string &id, const std::string &name) : m_ID(id), m_Name(name)
    {
        //
    }

    AppWindow::AppWindow(const std::string &id, const std::string &name, const std::string &icon) : m_ID(id), m_Name(name), m_Icon(icon)
    {
        SetIcon(m_Icon);
    }

    AppWindow::~AppWindow()
    {
        //
    }

    std::shared_ptr<RedockRequest> AppWindow::CreateRedockEvent(const std::string &parentWindow, DockEmplacement emplacement, const bool &fromSave, const std::string &appWindow, const bool &new_win)
    {
        std::shared_ptr<RedockRequest> req = std::make_shared<RedockRequest>();
        req->m_DockPlace = emplacement;
        req->m_ParentAppWindow = appWindow;
        req->m_ParentAppWindowHost = this->m_IdName;
        req->m_ParentWindow = parentWindow;
        req->m_FromSave = fromSave;
        req->m_FromNewWindow = new_win;
        return req;
    }

    void AppWindow::AttachOnNewWindow(ApplicationSpecification spec)
    {
        m_AttachRequest.m_Specification = spec;
        m_AttachRequest.m_AppWindowName = m_IdName;
        m_AttachRequest.m_IsFinished = false;
    }

    void AppWindow::AttachOnWindow(const std::string winname)
    {
        //
    }

    void AppWindow::SetDeleteCallback(const std::function<void()> &callback)
    {
        if(callback)
        {
            m_DeleteCallback = callback;
        }
    }

    void AppWindow::DeleteSignal()
    {
        /*if(m_DeleteCallback)
        {
            m_DeleteCallback();
        }*/
    }

    void AppWindow::CtxRender(std::vector<std::shared_ptr<RedockRequest>> *reqs, const std::string &winname)
    {
        if (!m_IsRendering)
        {
            return;
        }

        if (!m_Visible)
        {
            return;
        }

        if (m_Closable)
        {
            if (!m_CloseSignal)
            {
                if (this->m_CloseCallback)
                {
                    this->m_CloseCallback();
                }
                m_CloseSignal = true;
            }
        }

        if (!m_Opened)
        {
            m_CloseEvent;
            m_IsRendering = false;
        }

        std::shared_ptr<Window> window_instance;
        for (auto &win : Application::Get().m_Windows)
        {
            if (win->GetName() == winname)
            {
                window_instance = win;
            }
        }

        m_DockSpaceID = ImGui::GetID(m_HaveParentAppWindow ? "AppWindowDockspace" : "MainDockspace");

        if (!window_instance)
        {
            return;
        }

        if (!ImGui::DockBuilderGetNode(m_DockSpaceID))
        {
            ImGui::DockBuilderRemoveNode(m_DockSpaceID);
            ImGui::DockBuilderAddNode(m_DockSpaceID, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(m_DockSpaceID, ImGui::GetMainViewport()->Size);
            ImGui::DockBuilderFinish(m_DockSpaceID);
        }

        ImGuiWindow *currentWindow = ImGui::FindWindowByName(this->m_IdName.c_str());

        if (currentWindow && currentWindow->DockId == 0)
        {
            ImGui::SetNextWindowDockID(m_DockSpaceID, ImGuiCond_Always);
        }
        bool delete_all = false;

        if (reqs)
        {
            for (auto it = reqs->begin(); it != reqs->end();)
            {
                const auto &req = *it;

                if (req->m_IsObsolete)
                {
                    ++it;
                    continue;
                }

                ImGuiID parentDockID = m_DockSpaceID;
                ImGuiWindow *parentWindow = ImGui::FindWindowByName(req->m_ParentAppWindow.c_str());

                if (req->m_ParentAppWindowHost != this->m_IdName)
                {
                    ++it;
                    continue;
                }

                if (req->m_ParentAppWindowHost == req->m_ParentAppWindow)
                {
                    ++it;
                    continue;
                }

                if (!req->m_FromSave)
                {
                    Application::Get().m_IsDataSaved = false;
                }

                if (parentWindow)
                {
                    parentDockID = parentWindow->DockId;
                }

                switch (req->m_DockPlace)
                {
                case DockEmplacement::DockUp:
                    m_DockSpaceID = ImGui::DockBuilderSplitNode(parentDockID, ImGuiDir_Up, 0.5f, nullptr, &parentDockID);
                    break;
                case DockEmplacement::DockDown:
                    m_DockSpaceID = ImGui::DockBuilderSplitNode(parentDockID, ImGuiDir_Down, 0.5f, nullptr, &parentDockID);
                    break;
                case DockEmplacement::DockLeft:
                    m_DockSpaceID = ImGui::DockBuilderSplitNode(parentDockID, ImGuiDir_Left, 0.3f, nullptr, &parentDockID);
                    break;
                case DockEmplacement::DockRight:
                    m_DockSpaceID = ImGui::DockBuilderSplitNode(parentDockID, ImGuiDir_Right, 0.3f, nullptr, &parentDockID);
                    break;
                case DockEmplacement::DockFull:
                    m_DockSpaceID = parentDockID;
                    break;
                default:
                    ++it;
                    continue;
                }

                ImGui::SetNextWindowDockID(m_DockSpaceID, ImGuiCond_Always);

                if (req->m_FromNewWindow)
                {
                    delete_all = true;
                }

                it = reqs->erase(it);
            }

            if (delete_all)
            {
                for (auto &request : *reqs)
                {
                    if (request->m_ParentAppWindowHost == this->m_IdName)
                    {
                        request->m_IsObsolete = true;
                    }
                }
            }
        }

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;

        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 12));

        if (m_SaveMode && !m_Saved)
        {
            window_flags |= ImGuiWindowFlags_UnsavedDocument;
        }

        if (m_EnableMenuBar)
        {
            window_flags |= ImGuiWindowFlags_MenuBar;
        }

        if (m_EnableBottomBar)
        {
            window_flags |= ImGuiWindowFlags_BottomBar;
        }

        std::string window_name;

        if (m_Name.rfind("?loc:", 0) == 0)
        {
            std::string localeName = m_Name.substr(5);
            window_name = Application::Get().GetLocale(localeName) + "####" + localeName;
        }
        else
        {
            window_name = m_Name;
        }

        m_IdName = window_name;

        if (Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode != WindowRenderingMethod::SimpleWindow)
        {
            if (this->GetImage(m_Icon))
            {
                ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_Icon);
                if (m_Closable)
                {
                    ImGui::Begin(m_IdName.c_str(), texture, &m_CloseSignal, window_flags, ImVec2(m_InternalPaddingX, m_InternalPaddingY));
                }
                else
                {
                    ImGui::Begin(m_IdName.c_str(), texture, nullptr, window_flags, ImVec2(m_InternalPaddingX, m_InternalPaddingY));
                }
            }
            else
            {
                if (m_Closable)
                {
                    ImGui::Begin(m_IdName.c_str(), &m_CloseSignal, window_flags);
                }
                else
                {
                    ImGui::Begin(m_IdName.c_str(), nullptr, window_flags);
                }
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::BeginChild(ImGui::GetID("MainChild"), ImVec2(0, 0), false, window_flags);
        }

        // Prevent undock
        if (Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode == WindowRenderingMethod::DockingWindows || Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode == WindowRenderingMethod::TabWidows)
        {
            if (!ImGui::IsWindowDocked())
            {
                std::shared_ptr<Window> wind;

                for (auto &win : Application::Get().m_Windows)
                {
                    if (win->GetName() == winname)
                    {
                        wind = win;
                    }
                }

                Application::SetCurrentDragDropState(wind->drag_dropstate);

                Application::SetCurrentDragDropStateAppWindow("none");
                Application::SetCurrentDragDropStateWindow(winname);
                Application::SetCurrentDragDropStateAppWindowHost(this->m_IdName);
                Application::SetCurrentDragDropStateDraggingPlace(DockEmplacement::DockFull);

                Application::PushRedockEvent(wind->drag_dropstate);
            }
        }

        if (ImGui::BeginMenuBar())
        {
            float oldsize = ImGui::GetFont()->Scale;
            ImGui::GetFont()->Scale *= 0.85;
            ImGui::PushFont(ImGui::GetFont());

            float menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();
            float buttonHeight = 24;
            float offsetY = (menuBarHeight - buttonHeight) * 0.5f;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));

            float initialCursorPosY = ImGui::GetCursorPosY();
            ImGui::SetCursorPosY(initialCursorPosY + offsetY);

            if (m_MenubarLeft)
            {
                m_MenubarLeft();
            }

            float x_size = EstimateMenubarRightWidth();
            float right_pos = ImGui::GetWindowWidth() - x_size - ImGui::GetStyle().ItemSpacing.x * 2;

            ImGui::SetCursorPosX(right_pos);

            if (m_MenubarRight)
            {
                m_MenubarRight();
            }

            ImGui::PopStyleVar();
            ImGui::GetFont()->Scale = oldsize;
            ImGui::PopFont();

            ImGui::EndMenuBar();
        }

        ImGui::PopStyleVar(2);

        ImGuiContext *ctx = ImGui::GetCurrentContext();

        if (this->m_TabMenuCallback)
        {
            ImGui::GetCurrentWindow()->ContextMenuCallback = this->m_TabMenuCallback;
        }

        if (this->m_CloseCallback)
        {
            ImGui::GetCurrentWindow()->CloseCallback = this->m_CloseCallback;
        }

        ImGui::GetCurrentWindow()->Closable = this->m_Closable;
        ImGui::GetCurrentWindow()->Saved = this->m_Saved;
        ImGui::GetCurrentWindow()->DragDisabled = this->m_DisableDragging;
        ImGui::GetCurrentWindow()->ContextMenuDisabled = this->m_DisableContextMenu;

        std::shared_ptr<Window> wind;

        for (auto &win : Application::Get().m_Windows)
        {
            if (win->GetName() == winname)
            {
                wind = win;
            }
        }

        // Drag
        if (m_DockingMode || Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode == WindowRenderingMethod::DockingWindows)
        {
            if (ctx->DockTabStaticSelection.Pressed)
            {
                wind->drag_dropstate->DockIsDragging = true;
                wind->drag_dropstate->LastDraggingAppWindowHost = ctx->DockTabStaticSelection.TabName;
                Application::SetLastWindowPressed(wind->drag_dropstate->LastDraggingAppWindowHost);
                // wind->drag_dropstate->LastDraggingAppWindow = ctx->DockTabStaticSelection.TabName;
                wind->drag_dropstate->DragOwner = this->m_IdName;
                Application::SetCurrentDragDropState(wind->drag_dropstate);
            }

            // Drop
            if (Application::GetCurrentDragDropState())
            {
                if (Application::GetCurrentDragDropState()->DragOwner == this->m_IdName)
                {
                    if (!ctx->DockTabStaticSelection.Pressed)
                    {
                        // if (this->CheckWinParent(winname))
                        //{
                        if (Application::GetCurrentDragDropState()->LastDraggingPlace == DockEmplacement::DockBlank)
                        {
                            Application::GetCurrentDragDropState()->CreateNewWindow = true;
                        }
                        else
                        {
                            Application::PushRedockEvent(Application::GetCurrentDragDropState());
                        }
                        //}

                        /*if (m_HaveParentAppWindow)
                        {
                            AddWinParent(winname);
                            wind->drag_dropstate->LastDraggingAppWindowHaveParent = true;
                        }*/

                        wind->drag_dropstate->DockIsDragging = false;
                        wind->drag_dropstate->DragOwner = "none";
                    }
                }
            }
        }

        ImGuiID dockID = ImGui::GetID("AppWindowDockspace");
        float oldsize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= 0.84;
        ImGui::PushFont(ImGui::GetFont());

        ImVec4 grayColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        ImVec4 graySeparatorColor = ImVec4(0.4f, 0.4f, 0.4f, 0.5f);
        ImVec4 darkBackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
        ImVec4 lightBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, darkBackgroundColor);

        ImGui::PushStyleColor(ImGuiCol_Border, lightBorderColor);

        if (m_DockingMode)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 5.0f));
            ImGui::GetCurrentContext()->Style.DockSpaceMenubarPaddingY = 18.0f;

            ImGui::DockSpace(dockID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

            if (Application::GetDockIsDragging())
            {
                if (m_DockingMode)
                {
                    for (auto &win : Application::Get().m_AppWindows)
                    {
                        if (Application::GetCurrentDragDropState()->LastDraggingAppWindowHost == win->m_IdName)
                        {
                            if (win->m_HaveParentAppWindow)
                            {
                                if (win->m_ParentAppWindow->m_IdName == this->m_IdName)
                                {
                                    for (auto &winc : Application::Get().m_Windows)
                                    {
                                        if (winc->GetName() == winname)
                                        {
                                            Window::ShowDockingPreview(dockID, winc.get(), Application::GetCurrentDragDropState());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (auto &win : Application::Get().m_AppWindows)
            {
                if (win->m_HaveParentAppWindow)
                {
                    if (win->m_ParentAppWindow->m_IdName == this->m_IdName)
                    {
                        if (win->CheckWinParent(winname))
                        {
                            win->CtxRender(reqs, winname);
                        }
                    }
                }
            }
            ImGui::PopStyleVar(2);
        }
        else
        {
            this->m_Render();
        }

        if (ImGui::BeginBottomBar())
        {
            float oldsize = ImGui::GetFont()->Scale;
            ImGui::GetFont()->Scale *= 0.85;
            ImGui::PushFont(ImGui::GetFont());

            float menuBarHeight = ImGui::GetCurrentWindow()->BottomBarHeight();
            float buttonHeight = 24;
            float offsetY = (menuBarHeight - buttonHeight) * 0.5f;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 2));

            float initialCursorPosY = ImGui::GetCursorPosY();
            ImGui::SetCursorPosY(initialCursorPosY + offsetY);

            if (m_BottombarLeft)
            {
                m_BottombarLeft();
            }

            float x_size = EstimateMenubarRightWidth();
            float right_pos = ImGui::GetWindowWidth() - x_size - ImGui::GetStyle().ItemSpacing.x * 2;

            ImGui::SetCursorPosX(right_pos);

            if (m_BottombarRight)
            {
                m_BottombarRight();
            }

            ImGui::PopStyleVar();
            ImGui::GetFont()->Scale = oldsize;
            ImGui::PopFont();

            ImGui::EndBottomBar();
        }

        ImGui::PopStyleColor(2);

        ImGui::GetFont()->Scale = oldsize;
        ImGui::PopFont();

        if (Application::GetCurrentRenderedWindow()->m_Specifications.RenderMode != WindowRenderingMethod::SimpleWindow)
        {
            ImGui::End();
        }
        else
        {
            ImGui::EndChild();
            ImGui::PopStyleColor(2);
        }
    }

    void AppWindow::SetVisibility(const bool &new_visibility)
    {
        m_Visible = new_visibility;
    }

    bool AppWindow::CheckWinParent(const std::string &parentname)
    {
        if (this->m_WinParent == parentname)
        {
            return true;
        }

        return false;
    }

    void AppWindow::SetParentWindow(const std::string &parentname)
    {
        this->m_WinParent = parentname;
    }

    void AppWindow::SetClosable(const bool &is_closable)
    {
        this->m_Closable = is_closable;
    }

    void AppWindow::SetCloseCallback(std::function<void()> close_callback)
    {
        this->m_CloseCallback = close_callback;
    }

    void AppWindow::SetSaveMode(const bool &use_save_mode)
    {
        this->m_SaveMode = use_save_mode;
    }

    void AppWindow::SetSaved(const bool &new_state)
    {
        this->m_Saved = new_state;
    }

    void AppWindow::SetOpened(const bool &new_state)
    {
        this->m_Opened = new_state;
    }

    void AppWindow::SetDisableDragging(const bool &new_state)
    {
        this->m_DisableDragging = new_state;
    }

    void AppWindow::SetDisableContextMenu(const bool &new_state)
    {
        this->m_DisableContextMenu = new_state;
    }

    void AppWindow::SetInternalPaddingX(const float &new_padding)
    {
        this->m_InternalPaddingX = new_padding;
    }

    void AppWindow::SetInternalPaddingY(const float &new_padding)
    {
        this->m_InternalPaddingY = new_padding;
    }

    float AppWindow::EstimateMenubarRightWidth()
    {
        ImVec2 initialCursorPos = ImGui::GetCursorPos();
        ImGui::PushClipRect(ImVec2(0, 0), ImVec2(0, 0), false);
        ImGuiID id = ImGui::GetID("TempID");

        ImGui::PushID(id);
        ImGui::SetCursorPosX(0.0f);
        if (m_MenubarRight)
        {
            m_MenubarRight();
        }
        float width = ImGui::GetCursorPosX();
        ImGui::PopID();

        ImGui::PopClipRect();
        ImGui::SetCursorPos(initialCursorPos);

        return width;
    }

    void AppWindow::SetSimpleStorage(const std::string &key, const std::string &data, const bool &persistant)
    {
        std::shared_ptr<SimpleStorageItem> value = std::make_shared<SimpleStorageItem>(data, persistant);
        m_Storage[key] = value;
    }

    void AppWindow::SetWindowStorage(const std::string &key, const nlohmann::json &data, const bool &persistant)
    {
        std::shared_ptr<WindowStorageItem> value = std::make_shared<WindowStorageItem>(data, persistant);
        m_WindowStorage[key] = value;
    }

    std::shared_ptr<SimpleStorageItem> AppWindow::GetSimpleStorage(const std::string &key)
    {
        if (m_Storage.find(key) != m_Storage.end())
        {
            return m_Storage[key];
        }
        else
        {
            return nullptr;
        }
    }

    std::shared_ptr<WindowStorageItem> AppWindow::GetWindowStorage(const std::string &key)
    {
        if (m_WindowStorage.find(key) != m_WindowStorage.end())
        {
            return m_WindowStorage[key];
        }
        else
        {
            return nullptr;
        }
    }

    std::unordered_map<std::string, std::shared_ptr<WindowStorageItem>> AppWindow::DumpWindowStorage()
    {
        return m_WindowStorage;
    }
    std::unordered_map<std::string, std::shared_ptr<SimpleStorageItem>> AppWindow::DumpSimpleStorage()
    {
        return m_Storage;
    }

    void AppWindow::SetDefaultBehavior(DefaultAppWindowBehaviors behavior, const std::string &value)
    {
        m_DefaultBehaviors[behavior] = value;
    }

    std::string AppWindow::GetDefaultBehavior(DefaultAppWindowBehaviors behavior)
    {
        if (m_DefaultBehaviors.find(behavior) != m_DefaultBehaviors.end())
        {
            return m_DefaultBehaviors[behavior];
        }
        else
        {
            return "undefined";
        }
    }

    void AppWindow::SpawnInNewWindow()
    {
    }

    void AppWindow::SetRenderCallback(const std::function<void()> &render)
    {
        m_Render = render;
    }

    void AppWindow::SetLeftMenubarCallback(const std::function<void()> &right_menubar)
    {
        m_MenubarLeft = right_menubar;
        m_EnableMenuBar = true;
    }

    void AppWindow::SetRightMenubarCallback(const std::function<void()> &left_menubar)
    {
        m_MenubarRight = left_menubar;
        m_EnableMenuBar = true;
    }

    void AppWindow::SetLeftBottombarCallback(const std::function<void()> &left_menubar)
    {
        m_BottombarLeft = left_menubar;
        m_EnableBottomBar = true;
    }

    void AppWindow::SetRightBottombarCallback(const std::function<void()> &right_menubar)
    {
        m_BottombarRight = right_menubar;
        m_EnableBottomBar = true;
    }

    void AppWindow::SetIcon(const std::string &name)
    {
        m_Icon = name;
    }

    void AppWindow::SetDockingMode(const bool &use_docking)
    {
        m_DockingMode = use_docking;
    }

    void AppWindow::SetFetchedSaveData(const std::string &key, const std::string &value)
    {
        m_LastSaveData[key] = value;
    }

    void AppWindow::SetInstanciable()
    {
        m_AppWindowType = AppWindowTypes::InstanciableWindow;
    }

    std::string AppWindow::GetFetchedSaveData(const std::string &key)
    {
        if (m_LastSaveData.find(key) != m_LastSaveData.end())
        {
            return m_LastSaveData[key];
        }
        else
        {
            return "undefined";
        }
    }

    std::shared_ptr<Cherry::Image> AppWindow::GetImage(const std::string &path)
    {
        for (auto &win : Application::Get().m_Windows)
        {
            if (this->CheckWinParent(win->GetName()))
            {
                return win->get(path);
            }
        }
        return nullptr;
    }

    ImTextureID *AppWindow::GetTexture(const std::string &path)
    {
        for (auto &win : Application::Get().m_Windows)
        {
            if (this->CheckWinParent(win->GetName()))
            {
                static ImTextureID logoID = this->GetImage(m_Icon)->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                return &logoID;
            }
        }
        return nullptr;
    }

    void AppWindow::SetParent(const std::shared_ptr<AppWindow> &parent)
    {
        m_ParentAppWindow = parent;
        for (auto &appwin : Application::Get().m_AppWindows)
        {
            if (appwin->m_IdName == this->m_IdName)
            {
                parent->m_SubAppWindows.push_back(appwin);
            }
        }
        m_HaveParentAppWindow = true;
    }
}