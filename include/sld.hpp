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
#define SLD_API_INLINE          inline
#define SLD_API_INLINE_TEMPLATE template<typename type> inline
#define SLD_UTILITY             static constexpr

#ifndef assert
#   ifdef WIN32
#       define assert(expr) if(!expr) DebugBreak()
#   else
#       define assert(expr) if(!expr) *(int*)(NULL)=1
#   endif
#endif

namespace sld {

    //-------------------------------------------------------------------
    // PRIMITIVES
    //-------------------------------------------------------------------

    // signed integers
    typedef int8_t   s8;
    typedef int16_t  s16;
    typedef int32_t  s32;
    typedef int64_t  s64;

    // unsigned integers
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    // floats
    typedef float    f32;
    typedef double   f64;

    // booleans
    typedef int8_t   b8;
    typedef int16_t  b16;
    typedef int32_t  b32;
    typedef int64_t  b64;

    // chars
    typedef char     cchar;
    typedef wchar_t  wchar;

    // memory
    typedef uint8_t  byte;
    typedef intptr_t addr;
    typedef void*    vptr;
    typedef u8       pad8;
    typedef u16      pad16;
    typedef u32      pad32;
    typedef u64      pad64;

    // structured
    struct  s8_t    { s8    val; };
    struct  s16_t   { s16   val; };
    struct  s32_t   { s32   val; };
    struct  s64_t   { s64   val; };
    struct  u8_t    { u8    val; };
    struct  u16_t   { u16   val; };
    struct  u32_t   { u32   val; };
    struct  u64_t   { u64   val; };
    struct  f32_t   { f32   val; };
    struct  f64_t   { f64   val; };
    struct  b8_t    { b8    val; };
    struct  b16_t   { b16   val; };
    struct  b32_t   { b32   val; };
    struct  b64_t   { b64   val; };
    struct  byte_t  { byte  val; };
    struct  addr_t  { addr  val; };
    struct  vptr_t  { vptr  val; };

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

    //-------------------------------------------------------------------
    // MEMORY
    //-------------------------------------------------------------------

};

#endif //SLD_HPP
