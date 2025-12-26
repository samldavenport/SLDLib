#ifndef SLD_VECTOR_HPP
#define SLD_VECTOR_HPP

#include "sld.hpp"

namespace sld {

    struct vec2;
    struct vec3;

    void vec2_normalize                     (vec2&       v3);
    void vec2_magnitude                     (const vec2& v3,         f32&    m);
    void vec2_scalar_mul                    (vec2&       v2,   const f32     s);
    void vec2_scalar_div                    (vec2&       v2,   const f32     s);
    void vec2_scalar_mul_new                (vec2&       v2,   const f32     s, vec2& v2_new);
    void vec2_scalar_div_new                (vec2&       v2,   const f32     s, vec2& v2_new);
    void vec2_a_add_b                       (vec2&       v2_a, const vec2& v2_b);
    void vec2_a_sub_b                       (vec2&       v2_a, const vec2& v2_b);
    void vec2_a_dot_b                       (vec2&       v2_a, const vec2& v2_b, f32&    dot);
    void vec2_a_add_b_to_c                  (const vec2& v2_a, const vec2& v2_b, vec2& v2_c); 
    void vec2_a_sub_b_to_c                  (const vec2& v2_a, const vec2& v2_b, vec2& v2_c); 



    struct vec2 {
        union {
            struct {
                f32 x;
                f32 y;
            };
            f32 buffer[2];
        };
    };

    struct vec3 {
        union {
            struct {
                f32 x;
                f32 y;
                f32 z;
            };
            f32 buffer[3];
        };
    };
};

#endif //SLD_VECTOR_HPP