#ifndef SLD_VECTOR_HPP
#define SLD_VECTOR_HPP

#include "sld.hpp"

namespace sld {


    struct vec2 {
        union {
            struct {
                f32 x;
                f32 y;
            };
            buffer[2];
        };
    };

    struct vec3 {
        union {
            struct {
                f32 x;
                f32 y;
                f32 z;
            };
            buffer[3];
        };
    };
};

#endif //SLD_VECTOR_HPP