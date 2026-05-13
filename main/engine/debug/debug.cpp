//
//  debug.cpp
//  Main sources for Cherry debug tools
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "debug.hpp"

#include <iostream>

#include "./ui/devtool.hpp"
#ifdef CHERRY_DEBUG
namespace Cherry {

  static Tools::Devtool devtool;

  void Debugger::StartDevTools() {
    devtool.Start();
  }
  void Debugger::StopDevTools() {
    devtool.Stop();
  }

}  // namespace Cherry
#endif