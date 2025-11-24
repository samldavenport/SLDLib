#ifndef SLD_CSTR_HPP
#define SLD_CSTR_HPP

#include "sld.hpp"

#define SLD_API_INLINE_CSTR inline auto cstr::

namespace sld {
    
    //-------------------------------------------------------------------
    // CSTR API  | C-STRING | ASCII | UTF-8
    //-------------------------------------------------------------------

    struct cstr {
        cchar* chars;
        u32    size;

        inline void init         (cchar* chars, const u32 size);
        inline bool is_valid     (void);
        inline void assert_valid (void);
        inline void clear        (void);
        inline u32  length       (void);
        inline bool copy_to      (cchar* dst_chars, const u32 dst_size);
        inline bool copy_to      (cstr*  dst_cstr);
        inline bool copy_to      (cstr&  dst_cstr);
        inline bool copy_from    (const cchar* src_chars, const u32 src_size);
        inline bool copy_from    (const cstr*  src_cstr);
        inline bool copy_from    (const cstr&  src_cstr);
    };
    
    SLD_API_INLINE void
    cstr::init(
        cchar*    chars,
        const u32 size) { 
        
        assert(chars != NULL && size != 0);
        this->chars = chars;
        this->size  = size;
    }

    SLD_API_INLINE bool
    cstr::is_valid(
        void) {

        const bool is_valid = (
            this->chars != NULL &&
            this->size  != 0
        );
        if (is_valid) {
            this->chars[this->size - 1] = 0;
        }
        return(is_valid);
    }

    SLD_API_INLINE void
    cstr::assert_valid(
        void) {

        assert(this->is_valid());
    }

    SLD_API_INLINE void
    cstr::clear(
        void) {

        this->assert_valid();
        (void)memset(this->chars, 0, this->size);
    }


    SLD_API_INLINE u32
    cstr::length(
        void) {

        this->assert_valid();

        const u32 size = (u32)strnlen_s(
            this->chars,
            this->size
        );

        return(size);
    }

    SLD_API_INLINE bool
    cstr::copy_to(
        cchar*    dst_chars,
        const u32 dst_size) {

        this->assert_valid();

        const cchar*  src_chars = this->chars;
        const u32     src_size  = this->size; 
        const errno_t result    = strncpy_s(
            dst_chars, dst_size,
            src_chars, src_size
        );

        return(result == 0);
    }

    SLD_API_INLINE bool
    cstr::copy_to(
        cstr* dst_cstr) {

        assert(
            this->is_valid() &&
            dst_cstr != NULL &&
            dst_cstr->is_valid()
        );

        const cchar*  src_chars = this->chars;
        const u32     src_size  = this->size; 
        const errno_t result    = strncpy_s(
            dst_cstr->chars, dst_cstr->size,
            src_chars,       src_size
        );

        return(result == 0);
    }

    SLD_API_INLINE bool
    cstr::copy_to(
        cstr& dst_cstr) {
        
        assert(
            this->is_valid() &&
            dst_cstr.is_valid()
        );
        this->assert_valid();

        const cchar*  src_chars = this->chars;
        const u32     src_size  = this->size; 
        const errno_t result    = strncpy_s(
            dst_cstr.chars, dst_cstr.size,
            src_chars,      src_size
        );

        return(result == 0);
    }

    SLD_API_INLINE bool
    cstr::copy_from (
        const cchar* src_chars,
        const u32    src_size) {

        assert(
            this->is_valid()  &&
            src_chars != NULL &&
            src_size  != 0
        );

        cchar*        dst_chars  = this->chars;
        const u32     dst_size   = this->size; 
        const errno_t result     = strncpy_s(
            dst_chars, dst_size,
            src_chars, src_size
        );

        const bool did_copy = (result == 0); 
        return(did_copy);
    }

    SLD_API_INLINE bool
    cstr::copy_from(
        const cstr* src_cstr) {

        assert(
            this->is_valid() &&
            src_cstr != NULL);

    }

    SLD_API_INLINE bool
    cstr::copy_from(
        const cstr&  src_cstr) {

    }
};

#endif //SLD_CSTR_HPP