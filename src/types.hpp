#pragma once

#include <cstdint>

using s8  = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

const f64 PI   = 3.141592653589793238463;
const f32 PI32 = 3.141592653589793238463f;

inline f64 to_deg64(f64 r) { return 180.0 * r / PI; }
inline f32 to_deg  (f32 r) { return 180.0f * r / PI32; }
inline f64 to_rad64(f64 r) { return PI * r / 180.0; }
inline f32 to_rad  (f32 r) { return PI32 * r / 180.0f; }
