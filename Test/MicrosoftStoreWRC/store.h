#pragma once
#ifdef MICROSOFTSTOREWRC_EXPORTS
#define MICROSOFTSTOREWRC_API __declspec(dllexport)
#else
#define MICROSOFTSTOREWRC_API __declspec(dllimport)
#endif


	extern "C" MICROSOFTSTOREWRC_API void helloWorld();

