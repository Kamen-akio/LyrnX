#pragma once
#include <gdiplus.h>

using namespace Gdiplus;

#ifndef _CONTROL_NATIVE_H_
#define _CONTROL_NATIVE_H_

namespace Control {
typedef void __stdcall templClickCallback();

typedef enum { Any, Click, User } EventHandlerID;

typedef enum { MouseHover = 1, MouseDown } ObjectFlagID;

typedef struct __tagRenderContext {
  /*
  * As Control::screen class instance;
  */
  void* screen;
  Graphics* graphics;
  RectF targetRect;
  RectF* presentRect;
} RenderContext;

}

#endif  // !_CONTROL_NATIVE_H_
