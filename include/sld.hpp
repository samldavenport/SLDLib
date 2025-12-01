#ifndef SLD_HPP
#define SLD_HPP

#include <Windows.h>

#include <cstdint>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#define SLD_API
#define SLD_INLINE              inline
#define SLD_INTERNAL_INLINE     static inline
#define SLD_INTERNAL            static
#define SLD_GLOBAL              static
#define SLD_API_INLINE          inline
#define SLD_API_INLINE_TEMPLATE template<typename type> inline
#define SLD_UTILITY             static constexpr
#define SLD_API_OS                          extern // os api declaration
#define SLD_API_OS_FUNC                     static // os api function
#define SLD_API_OS_INTERNAL                 static // internal os function

#ifndef assert
#   ifdef WIN32
#       define assert(expr) if(!expr) DebugBreak()
#   else
#       define assert(expr) if(!expr) *(int*)(NULL)=1
#   endif
#endif

namespace sld {

    //-------------------------------------------------------------------
    // PRIMITIVE TYPES
    //-------------------------------------------------------------------

    // signed integers
    using s8  = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;

    // unsigned integers
    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    // floats
    using f32 = float;
    using f64 = double;

    // booleans
    using b8  = int8_t;
    using b16 = int16_t;
    using b32 = int32_t;
    using b64 = int64_t;

    // chars
    using cchar = char;
    using wchar = wchar_t;

    // memory
    using byte  = uint8_t;
    using addr  = intptr_t;
    using vptr  = void*;
    using pad8  = u8;
    using pad16 = u16;
    using pad32 = u32;

    //-------------------------------------------------------------------
    // STRONG TYPES
    //-------------------------------------------------------------------

    struct u32_t  { u32  val; };
    struct s32_t  { s32  val; };
    struct vptr_t { vptr val; };

    //-------------------------------------------------------------------
    // SIZE UTILITIES
    //-------------------------------------------------------------------

    SLD_UTILITY const u64 size_kilobytes   (const u64 n_kilobytes)               { return (n_kilobytes * 1024);                                                         }
    SLD_UTILITY const u64 size_megabytes   (const u64 n_megabytes)               { return (n_megabytes * 1024 * 1024);                                                  }
    SLD_UTILITY const u64 size_gigabytes   (const u64 n_gigabytes)               { return (n_gigabytes * 1024 * 1024 * 1024);                                           }
    SLD_UTILITY const u64 size_align       (const u64 size, const u64 alignment) { return ((alignment == 0) ? size : (size + alignment - 1) / (alignment * alignment)); }
    SLD_UTILITY const u64 size_align_pow_2 (const u64 size, const u64 alignment) { return ((alignment == 0) ? size : (size + alignment - 1) & ~(alignment - 1));        }
    SLD_UTILITY bool      size_is_pow_2    (const u64 size)                      { return (((size > 0) && ((size & (size - 1)) == 0)));                                 }

    SLD_UTILITY const u64
    size_round_up_pow2(
        const u64 size) {

        u64 pow2 = size;

        if (!size_is_pow_2(size)) {
            pow2 |= pow2 >> 1;
            pow2 |= pow2 >> 2;
            pow2 |= pow2 >> 4;
            pow2 |= pow2 >> 8;
            pow2 |= pow2 >> 16;
            pow2 |= pow2 >> 32;
            pow2++;
        }

        return(pow2);
    }

    //-------------------------------------------------------------------
    // BITWISE UTILITIES
    //-------------------------------------------------------------------

    SLD_UTILITY u32  bit_value     (const u32 bit)                                     { return (1 << bit);                                            }
    SLD_UTILITY bool bit_test      (const u32 bit, const u32 value)                    { return ((value >> bit) & 1);                                  }
    SLD_UTILITY void bit_set_high  (const u32 bit, u32&      value)                    { value |=  (1 << bit);                                         }
    SLD_UTILITY void bit_set_low   (const u32 bit, u32&      value)                    { value &= ~(1 << bit);                                         }
    SLD_UTILITY void bit_toggle    (const u32 bit, u32&      value)                    { value ^=  (1 << bit);                                         }
    SLD_UTILITY void bit_set       (const u32 bit, u32&      value, const bool state)  { value = (state) ? (value | (1 << bit)) : value & ~(1 << bit); }

    SLD_UTILITY bool bit_mask_test (const u32 value, const u32 mask)                   { return((value & mask) > 0);                         }
    SLD_UTILITY void bit_mask_and  (u32& value,      const u32 mask)                   { (value |=  mask);                                   }
    SLD_UTILITY void bit_mask_or   (u32& value,      const u32 mask)                   { (value &= ~mask);                                   }
};

#endif //SLD_HPP
