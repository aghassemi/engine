// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_LIB_UI_PAINTING_GRADIENT_H_
#define FLUTTER_LIB_UI_PAINTING_GRADIENT_H_

#include "flutter/lib/ui/painting/shader.h"
#include "flutter/tonic/dart_wrappable.h"
#include "flutter/tonic/float32_list.h"
#include "flutter/tonic/int32_list.h"
#include "third_party/skia/include/effects/SkGradientShader.h"

namespace blink {
class DartLibraryNatives;

template <>
struct DartConverter<SkShader::TileMode>
    : public DartConverterInteger<SkShader::TileMode> {};

static_assert(SkShader::kTileModeCount == 3, "Need to update tile mode enum");

class CanvasGradient : public Shader {
  DEFINE_WRAPPERTYPEINFO();
 public:
  ~CanvasGradient() override;
  static scoped_refptr<CanvasGradient> Create();

  void initLinear(const Float32List& end_points,
                  const Int32List& colors,
                  const Float32List& color_stops,
                  SkShader::TileMode tile_mode);

  void initRadial(double center_x,
                  double center_y,
                  double radius,
                  const Int32List& colors,
                  const Float32List& color_stops,
                  SkShader::TileMode tile_mode);

  static void RegisterNatives(DartLibraryNatives* natives);

 private:
  CanvasGradient();
};

} // namespace blink

#endif  // FLUTTER_LIB_UI_PAINTING_GRADIENT_H_
