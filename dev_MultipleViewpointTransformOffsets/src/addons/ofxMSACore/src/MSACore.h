#pragma once

// uncomment only ONE of the below #defines
// better to do this via project settings, but AFAIK neither openFrameworks or Cinder have a global #define

#define MSA_HOST_OPENFRAMEWORKS
//#define MSA_HOST_CINDER


//---------------------

#if defined( MSA_HOST_OPENFRAMEWORKS )
#include "MSACore-OF.h"

#elif defined( MSA_HOST_CINDER )
#include "MSACore-Cinder.h"

#else
This will give an error. you need to make sure a host is #defined either above, or in project settings
#endif

#include "MSACoreCommon.h"


