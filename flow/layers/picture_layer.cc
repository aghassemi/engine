// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flow/layers/picture_layer.h"

#include "base/logging.h"
#include "flow/checkerboard.h"
#include "flow/raster_cache.h"

namespace flow {

// TODO(abarth): Make this configurable by developers.
const bool kDebugCheckerboardRasterizedLayers = false;

PictureLayer::PictureLayer() {
}

PictureLayer::~PictureLayer() {
}

void PictureLayer::Preroll(PrerollContext* context,
                           const SkMatrix& matrix) {
  image_ = context->raster_cache.GetPrerolledImage(
      context->gr_context, picture_.get(), matrix);
  context->child_paint_bounds =
      picture_->cullRect().makeOffset(offset_.x(), offset_.y());
}

void PictureLayer::Paint(PaintContext& context) {
  DCHECK(picture_);

  if (image_) {
    TRACE_EVENT1("flutter", "PictureLayer::Paint", "image", "prerolled");
    SkRect rect = picture_->cullRect().makeOffset(offset_.x(), offset_.y());
    context.canvas.drawImageRect(image_.get(), rect, nullptr,
                                 SkCanvas::kFast_SrcRectConstraint);
    if (kDebugCheckerboardRasterizedLayers)
      DrawCheckerboard(&context.canvas, rect);
  } else {
    TRACE_EVENT1("flutter", "PictureLayer::Paint", "image", "normal");
    SkAutoCanvasRestore save(&context.canvas, true);
    context.canvas.translate(offset_.x(), offset_.y());
    context.canvas.drawPicture(picture_.get());
  }
}

}  // namespace flow
