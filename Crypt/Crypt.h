#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>
#include <string>

#include "PluginInterface.h"

using namespace System;

#define DLL_VERSION "1.4.0"
#define DLLFUNCTION  extern "C" __declspec(dllexport)

// Entry point when this DLL is loaded by Orion
DLLFUNCTION void Initialize(struct orion_plugin *plugin);
