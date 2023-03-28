#pragma once

enum ErrCorrLevels { L, M, Q, H };
constexpr int MIN_QR_CODE_VERSION = 1;
constexpr int MAX_QR_CODE_VERSION = 40;
constexpr int  MIN_MASK_PATTERN_CODE = 0;
constexpr int  MAX_MASK_PATTERN_CODE = 7;
typedef unsigned __int16 uint16;
constexpr int TYPE_SIZE = 2;
constexpr uint16 IMMUTABLE_BIT_VAL = ~static_cast<uint16>(0);
constexpr int NO_PATTERN_BASE = -1;