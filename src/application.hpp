#pragma once

#ifdef WL_HEADLESS
	#include "uikit/app_headless.h"
#else
	#include "../platform/engine/app.hpp"
#endif
