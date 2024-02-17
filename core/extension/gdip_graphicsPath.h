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
    return SetStatus(status);
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
    return SetStatus(status);
  }

  Status AddBezierRounded(int radius,
                          IN REAL left,
                          IN REAL top,
                          IN REAL width,
                          IN REAL height) {
    return SetStatus(
        AddBezierRounded(radius, Point(left, top), Size(width, height)));
  }

  Status AddBezierRounded(int radius,
                          IN int left,
                          IN int top,
                          IN int width,
                          IN int height) {
    return SetStatus(
        AddBezierRounded(radius, Point(left, top), Size(width, height)));
  }

  Status AddBezierRounded(int radius, Rect rect) {
    return SetStatus(AddBezierRounded(radius, Point(rect.X, rect.Y),
                                      Size(rect.Width, rect.Height)));
  }

  Status AddBezierRounded(int radius, RectF rect) {
    return SetStatus(AddBezierRounded(radius, PointF(rect.X, rect.Y),
                                      SizeF(rect.Width, rect.Height)));
  }
};

#endif  // _EXTENSTION_GDIP_GRAPHICSPATH_H_