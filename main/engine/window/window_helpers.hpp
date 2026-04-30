//
//  window_helpers.hpp
//  Helpers for window features of ImGui (desktop environment windows)
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <main/engine/app/app.hpp>

struct ImGuiDockPreviewData {
  ImGuiDockNode FutureNode;
  bool IsDropAllowed;
  bool IsCenterAvailable;
  bool IsSidesAvailable;    // Hold your breath, grammar freaks..
  bool IsSplitDirExplicit;  // Set when hovered the drop rect (vs. implicit
                            // SplitDir==None when hovered the window)
  ImGuiDockNode *SplitNode;
  ImGuiDir SplitDir;
  float SplitRatio;
  ImRect DropRectsDraw[ImGuiDir_COUNT + 1];  // May be slightly different from hit-testing drop
                                             // rects used in DockNodeCalcDropRects()

  ImGuiDockPreviewData() : FutureNode(0) {
    IsDropAllowed = IsCenterAvailable = IsSidesAvailable = IsSplitDirExplicit = false;
    SplitNode = NULL;
    SplitDir = ImGuiDir_None;
    SplitRatio = 0.f;
    for (int n = 0; n < IM_ARRAYSIZE(DropRectsDraw); n++)
      DropRectsDraw[n] = ImRect(+FLT_MAX, +FLT_MAX, -FLT_MAX, -FLT_MAX);
  }
};
