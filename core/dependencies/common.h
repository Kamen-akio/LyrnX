#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

// prprpr, i dont know why.

#include <chrono>
#include <exception>
#include <string>
#include <regex>

#include "interface.h"

#define IS_FAIL(state)                                              \
  {                                                                 \
    if (!state)                                                     \
      throw new std::exception("Oops, error. More see debugger. "); \
  };