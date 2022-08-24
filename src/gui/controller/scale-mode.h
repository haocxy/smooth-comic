#pragma once


namespace myapp {

using ScaleModeBaseType = int;

enum class ScaleMode : ScaleModeBaseType {
    RawSize,
    AutoFitAreaSize,
    AutoFitAreaWidth,
    AutoFitAreaHeight,
    FixWidthByRatio,
    FixHeightByRatio,
    FixWidthByPixel,
    FixHeightByPixel
};

}
