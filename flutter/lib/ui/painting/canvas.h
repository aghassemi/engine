// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_CANVAS_H_
#define FLUTTER_LIB_UI_PAINTING_CANVAS_H_

#include "base/memory/ref_counted.h"
#include "flutter/lib/ui/painting/paint.h"
#include "flutter/lib/ui/painting/path.h"
#include "flutter/lib/ui/painting/picture.h"
#include "flutter/lib/ui/painting/picture_recorder.h"
#include "flutter/lib/ui/painting/rrect.h"
#include "flutter/tonic/dart_wrappable.h"
#include "flutter/tonic/float32_list.h"
#include "flutter/tonic/float64_list.h"
#include "flutter/tonic/int32_list.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace blink {
class CanvasImage;
class DartLibraryNatives;

template <>
struct DartConverter<SkCanvas::PointMode>
    : public DartConverterInteger<SkCanvas::PointMode> {};

template <>
struct DartConverter<SkCanvas::VertexMode>
    : public DartConverterInteger<SkCanvas::VertexMode> {};

class Canvas : public base::RefCountedThreadSafe<Canvas>, public DartWrappable {
  DEFINE_WRAPPERTYPEINFO();
 public:
  static scoped_refptr<Canvas> Create(PictureRecorder* recorder,
                                      double left,
                                      double top,
                                      double right,
                                      double bottom);

  ~Canvas() override;

  void save();
  void saveLayerWithoutBounds(const Paint& paint,
                              const PaintData& paint_data);
  void saveLayer(double left,
                 double top,
                 double right,
                 double bottom,
                 const Paint& paint,
                 const PaintData& paint_data);
  void restore();
  int getSaveCount();

  void translate(double dx, double dy);
  void scale(double sx, double sy);
  void rotate(double radians);
  void skew(double sx, double sy);
  void transform(const Float64List& matrix4);
  void setMatrix(const Float64List& matrix4);

  void clipRect(double left,
                double top,
                double right,
                double bottom);
  void clipRRect(const RRect& rrect);
  void clipPath(const CanvasPath* path);

  void drawColor(SkColor color, SkXfermode::Mode transfer_mode);
  void drawLine(double x1,
                double y1,
                double x2,
                double y2,
                const Paint& paint,
                const PaintData& paint_data);
  void drawPaint(const Paint& paint,
                 const PaintData& paint_data);
  void drawRect(double left,
                double top,
                double right,
                double bottom,
                const Paint& paint,
                const PaintData& paint_data);
  void drawRRect(const RRect& rrect,
                 const Paint& paint,
                 const PaintData& paint_data);
  void drawDRRect(const RRect& outer,
                  const RRect& inner,
                  const Paint& paint,
                  const PaintData& paint_data);
  void drawOval(double left,
                double top,
                double right,
                double bottom,
                const Paint& paint,
                const PaintData& paint_data);
  void drawCircle(double x,
                  double y,
                  double radius,
                  const Paint& paint,
                  const PaintData& paint_data);
  void drawPath(const CanvasPath* path,
                const Paint& paint,
                const PaintData& paint_data);
  void drawImage(const CanvasImage* image,
                 double x,
                 double y,
                 const Paint& paint,
                 const PaintData& paint_data);
  void drawImageRect(const CanvasImage* image,
                     double src_left,
                     double src_top,
                     double src_right,
                     double src_bottom,
                     double dst_left,
                     double dst_top,
                     double dst_right,
                     double dst_bottom,
                     const Paint& paint,
                     const PaintData& paint_data);
  void drawImageNine(const CanvasImage* image,
                     double center_left,
                     double center_top,
                     double center_right,
                     double center_bottom,
                     double dst_left,
                     double dst_top,
                     double dst_right,
                     double dst_bottom,
                     const Paint& paint,
                     const PaintData& paint_data);
  void drawPicture(Picture* picture);

  // The paint argument is first for the following functions because Paint
  // unwraps a number of C++ objects. Once we create a view unto a
  // Float32List, we cannot re-enter the VM to unwrap objects. That means we
  // either need to process the paint argument first.

  void drawPoints(const Paint& paint,
                  const PaintData& paint_data,
                  SkCanvas::PointMode point_mode,
                  const Float32List& points);

  void drawVertices(const Paint& paint,
                    const PaintData& paint_data,
                    SkCanvas::VertexMode vertex_mode,
                    const Float32List& vertices,
                    const Float32List& texture_coordinates,
                    const Int32List& colors,
                    SkXfermode::Mode transfer_mode,
                    const Int32List& indices);

  void drawAtlas(const Paint& paint,
                 const PaintData& paint_data,
                 CanvasImage* atlas,
                 const Float32List& transforms,
                 const Float32List& rects,
                 const Int32List& colors,
                 SkXfermode::Mode transfer_mode,
                 const Float32List& cull_rect);

  SkCanvas* canvas() const { return canvas_; }
  void Clear();
  bool IsRecording() const;

  static void RegisterNatives(DartLibraryNatives* natives);

 private:
  explicit Canvas(SkCanvas* canvas);

  // The SkCanvas is supplied by a call to SkPictureRecorder::beginRecording,
  // which does not transfer ownership.  For this reason, we hold a raw
  // pointer and manually set to null in Clear.
  SkCanvas* canvas_;
};

} // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_CANVAS_H_
