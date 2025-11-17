#ifndef SLD_GRAPHICS_HPP
#define SLD_GRAPHICS_HPP

#include "sld.hpp"

namespace sld {

    constexpr u32 COLOR_CHANNEL_MAX_VALUE = 255;
    constexpr f32 COLOR_NORMAL_FACTOR     = (1.0f / (f32)COLOR_CHANNEL_MAX_VALUE);

    struct color_u32_t;
    struct color_f128_t;

    struct color_u32_t {
        union {
            u32 rgba;
            struct {
                byte r;
                byte g;
                byte b;
                byte a;
            };
        };
    };

    struct color_f128_t {
        union {
            f32 rgba[4];
            struct {
                f32 r;
                f32 g;
                f32 b;
                f32 a;
            };
        };
    };

    SLD_INLINE void 
    color_u32_normalize(
        const color_u32_t* in_color_u32,
        color_f128_t*     out_color_f128) {

        out_color_f128->r = (in_color_u32->r * COLOR_NORMAL_FACTOR);
        out_color_f128->g = (in_color_u32->g * COLOR_NORMAL_FACTOR);
        out_color_f128->b = (in_color_u32->b * COLOR_NORMAL_FACTOR);
        out_color_f128->a = (in_color_u32->a * COLOR_NORMAL_FACTOR);
    }
    
    SLD_INLINE void 
    color_f128_denormalize(
        const color_f128_t* in_color_f128,
        color_u32_t*       out_color_u32) {

        constexpr f32 factor = (f32)COLOR_CHANNEL_MAX_VALUE;
        out_color_u32->r = (in_color_f128->r * factor);
        out_color_u32->g = (in_color_f128->g * factor);
        out_color_u32->b = (in_color_f128->b * factor);
        out_color_u32->a = (in_color_f128->a * factor);
    }
};

#endif //SLD_GRAPHICS_HPP