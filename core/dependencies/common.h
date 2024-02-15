#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <chrono>
#include <exception>
#include <regex>
#include <string>
#include "interface.h"

#define IS_FAIL(state)                                              \
  {                                                                 \
    if (!state)                                                     \
      throw new std::exception("Oops, error. More see debugger. "); \
  };