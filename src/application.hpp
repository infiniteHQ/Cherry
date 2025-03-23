#pragma once

#ifdef WL_HEADLESS
	#include "cherry/app_headless.h"
#else
	#include "../platform/engine/app.hpp"
#endif
