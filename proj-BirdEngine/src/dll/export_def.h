#pragma once

#ifdef BIRDENGINE_EXPORTS
#define BIRDENGINE_API __declspec(dllexport)
#else
#define BIRDENGINE_API __declspec(dllimport)
#endif
