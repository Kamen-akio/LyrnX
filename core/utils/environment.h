#pragma once
#include "../dependencies/common.h"

#ifndef _UTILS_ENV_H_
#define _UTILS_ENV_H_

namespace Environment {

struct EnvProps {
  bool EnableDebug;
  bool EnableBlur;
  bool EnableRoundedFrame;
};

extern EnvProps props;

EnvProps GetEnv();
EnvProps* GetEnvRaw();
}  // namespace Environment

#endif  // !_UTILS_ENV_H_
