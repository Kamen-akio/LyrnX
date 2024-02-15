#pragma once
#include "../dependencies/render.h"

using namespace Gdiplus;

#ifndef _EXTENSTION_GDIP_GRAPHICSPATH_H_
#define _EXTENSTION_GDIP_GRAPHICSPATH_H_

class PathExt : public GraphicsPath {
 public:
  PathExt(IN FillMode fillMode = FillModeAlternate) : GraphicsPath(fillMode){};

  PathExt(IN const PointF* points,
          IN const BYTE* types,
          IN INT count,
          IN FillMode fillMode = FillModeAlternate)
      : GraphicsPath(points, types, count, fillMode){};

  PathExt(IN const Point* points,
          IN const BYTE* types,
          IN INT count,
          IN FillMode fillMode = FillModeAlternate)
      : GraphicsPath(points, types, count, fillMode){};

 public:
  Status AddBezierRounded(int radius,
                          IN const PointF& pos,
                          IN const SizeF& size) {
    Status status{};
    status = AddBezier(pos.X, pos.Y + radius, pos.X, pos.Y, pos.X, pos.Y,
                       pos.X + radius, pos.Y);

    status = AddBezier(pos.X + size.Width - radius, pos.Y, pos.X + size.Width,
                       pos.Y, pos.X + size.Width, pos.Y, pos.X + size.Width,
                       pos.Y + radius);

    status = AddBezier(pos.X + size.Width, pos.Y + size.Height - radius,
                       pos.X + size.Width, pos.Y + size.Height,
                       pos.X + size.Width, pos.Y + size.Height,
                       pos.X + size.Width - radius, pos.Y + size.Height);

    status = AddBezier(pos.X + radius, pos.Y + size.Height, pos.X,
                       pos.Y + size.Height, pos.X, pos.Y + size.Height, pos.X,
                       pos.Y + size.Height - radius);

    status = CloseFigure();
    return status;
  }

  Status AddBezierRounded(int radius,
                          IN const Point& pos,
                          IN const Size& size) {
    Status status{};
    status = AddBezier(pos.X, pos.Y + radius, pos.X, pos.Y, pos.X, pos.Y,
                       pos.X + radius, pos.Y);

    status = AddBezier(pos.X + size.Width - radius, pos.Y, pos.X + size.Width,
                       pos.Y, pos.X + size.Width, pos.Y, pos.X + size.Width,
                       pos.Y + radius);

    status = AddBezier(pos.X + size.Width, pos.Y + size.Height - radius,
                       pos.X + size.Width, pos.Y + size.Height,
                       pos.X + size.Width, pos.Y + size.Height,
                       pos.X + size.Width - radius, pos.Y + size.Height);

    status = AddBezier(pos.X + radius, pos.Y + size.Height, pos.X,
                       pos.Y + size.Height, pos.X, pos.Y + size.Height, pos.X,
                       pos.Y + size.Height - radius);

    status = CloseFigure();
    return status;
  }

  Status AddBezierRounded(int radius,
                          IN REAL left,
                          IN REAL top,
                          IN REAL width,
                          IN REAL height) {
    Point pos(left, top);
    Size size(width, height);
    Status status{};
    return AddBezierRounded(radius, pos, size);
  }

  Status AddBezierRounded(int radius,
                          IN int left,
                          IN int top,
                          IN int width,
                          IN int height) {
    Point pos(left, top);
    Size size(width, height);
    return AddBezierRounded(radius, pos, size);
  }
};

#endif  // _EXTENSTION_GDIP_GRAPHICSPATH_H_