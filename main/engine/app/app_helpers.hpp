//
//  app_helpers.hpp
//  Helpers and macros for main application features of Cherry
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#ifndef CHERRY_APP_HELPERS_HPP
#define CHERRY_APP_HELPERS_HPP

namespace Cherry {

  enum class ProcessCallback { ON_INITIALIZATION_FINISHED };

  enum class CherryKey {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,
    ESCAPE,
    SPACE,
    ENTER,
    BACKSPACE,
    KEY_DELETE,
    TAB,
    SHIFT,
    CTRL,
    ALT,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12
  };

}  // namespace Cherry

#endif  // CHERRY_APP_HELPERS_HPP