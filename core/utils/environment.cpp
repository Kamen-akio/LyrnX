#include "environment.h"

Environment::EnvProps Environment::props{};

Environment::EnvProps Environment::GetEnv() {
  return props;
}

Environment::EnvProps* Environment::GetEnvRaw() {
  return &props;
}