#pragma once
#include<string>
#ifdef MSSTORE_EXPORTS
#define MSSTORE_API __declspec(dllexport)
#else
#define MSSTORE_API __declspec(dllimport)
#endif

using namespace std;
extern "C" MSSTORE_API std::string helloWorld();