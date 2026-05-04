//
//  hooks_helpers.hpp
//  Helpers and structures/classes for Cherry hook system
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#ifndef HOOKS_HELPERS_HPP
#define HOOKS_HELPERS_HPP

namespace Cherry {

  enum class ExecuteHookOn { Frame, Second, Condition };

  class Hook {
   public:
    Hook();

   private:
    ExecuteHookOn m_ExecuteHookOn;
  };
}  // namespace Cherry

#endif  // HOOKS_HPP