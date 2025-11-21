#ifndef SLD_BUFFER_HPP
#define SLD_BUFFER_HPP

#include "sld.hpp"
#include "sld-memory.hpp"
#include "sld-arena.hpp"

#define SLD_API_INLINE_BUFFER inline auto buffer::

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    struct buffer {

        //members
        byte* data;
        u32   size;
        u32   length;

        // methods
        inline void init         (byte* data, const u32 size);
        inline bool is_valid     (void) const;
        inline void assert_valid (void) const;
        inline void reset        (void);
        inline u32  append_from  (const byte*   src_data, const u32 src_length);
        inline u32  append_from  (const buffer* src_buffer);
        inline u32  append_from  (const buffer& src_buffer);
        inline u32  append_to    (buffer*       dst_buffer);
        inline u32  append_to    (buffer&       dst_buffer);
        inline u32  copy_from    (const byte*   src_data, const u32 src_length);  
        inline u32  copy_to      (byte*         dst_data, const u32 dst_length) const;
        inline u32  copy_to      (buffer*       dst_buffer);  
        inline u32  copy_to      (buffer&       dst_buffer); 

        // operators
        inline byte&       operator[] (u32 index);
        inline const byte& operator[] (u32 index) const; 
    };
    
    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    // methods
    SLD_API_INLINE_BUFFER
    init(
        byte*     data,
        const u32 size) -> void {

        assert(data != NULL && size != 0);

        this->data   = data;
        this->size   = size;
        this->length = 0;
    }

    SLD_API_INLINE_BUFFER
    is_valid(
        void) const -> bool  {

        const bool is_valid = (
            this->data   != NULL &&
            this->size   != 0    &&
            this->length <= this->size
        );
        return(is_valid);
    }

    SLD_API_INLINE_BUFFER
    assert_valid(
        void) const -> void  {

        assert(this->is_valid());
    }

    SLD_API_INLINE_BUFFER
    reset(
        void) -> void {
        
        this->assert_valid();
        this->length = 0;
    }

    SLD_API_INLINE_BUFFER
    append_from(
        const byte* src_data,
        const u32   src_length) -> u32  {

        assert(
            this->is_valid()   &&
            src_data   != NULL &&
            src_length != 0
        );

        const u32 remaining     = (this->size - this->length);
        const u32 append_length = (remaining >= src_length)
            ? src_length
            : src_length - remaining;

        if (append_length > 0) {

            void* dst_data = (void*)(this->data + this->length);
            memcpy(dst_data, src_data, append_length);
        }
        return(append_length);
    }

    SLD_API_INLINE_BUFFER
    append_from(
        const buffer* src_buffer) -> u32  {
        
        assert(
            this->is_valid()   &&
            src_buffer != NULL &&
            src_buffer->is_valid()
        );

        const u32 append_length = this->append_from(
            src_buffer->data,
            src_buffer->length
        );
        this->length += append_length;
        return(append_length);
    }

    SLD_API_INLINE_BUFFER
    append_from(
        const buffer& src_buffer) -> u32  {
        
        assert(
            this->is_valid()   &&
            src_buffer.is_valid()
        );

        const u32 append_length = this->append_from(
            src_buffer.data,
            src_buffer.length
        );

        this->length += append_length;
        return(append_length);
    }

    SLD_API_INLINE_BUFFER
    append_to(
        buffer* dst_buffer) -> u32 {

        assert(
            this->is_valid()   &&
            dst_buffer != NULL &&
            dst_buffer->is_valid()
        );

        const u32 append_length = dst_buffer->append_from(
            this->data,
            this->length
        );

        return(append_length);
    }

    SLD_API_INLINE_BUFFER
    append_to(
        buffer& dst_buffer) -> u32 {

        assert(
            this->is_valid()   &&
            dst_buffer.is_valid()
        );

        const u32 append_length = dst_buffer.append_from(
            this->data,
            this->length
        );
        
        return(append_length);
    }

    SLD_API_INLINE_BUFFER
    copy_from(
        const byte* src_data,
        const u32   src_length) -> u32  {

        assert(
            this->is_valid()   &&
            src_data   != NULL &&
            src_length != 0
        );

        const u32 copy_length = (this->size >= src_length)
            ? src_length
            : src_length - this->size;
        
        if (copy_length > 0) {

            void* dst_data = (void*)this->data;
            memcpy(dst_data, src_data, copy_length);
        }
        return(copy_length);
    }

    SLD_API_INLINE_BUFFER
    copy_to(
        byte*     dst_data,
        const u32 dst_length) const -> u32  {

        assert(
            this->is_valid()   &&
            dst_data   != NULL &&
            dst_length != 0
        );

        const u32 copy_length = (dst_length >= this->length)
            ? this->length
            : this->length - dst_length;
        
        if (copy_length > 0) {
            const void* src_data = this->data;
            memcpy(dst_data, src_data, copy_length);
        }
        return(copy_length);
    }

    SLD_API_INLINE_BUFFER
    copy_to(
        buffer* dst) -> u32 {

        assert(
            this->is_valid() &&
            dst != NULL      &&
            dst->is_valid()
        );

        dst->length = this->copy_to(
            dst->data,
            this->length
        );
        return(dst->length);
    }
    
    SLD_API_INLINE_BUFFER
    copy_to(
        buffer& dst) -> u32 {

        assert(
            this->is_valid() &&
            dst.is_valid()
        );

        dst.length = this->copy_to(
            dst.data,
            this->length
        );
        return(dst.length);
    }

    //-------------------------------------------------------------------
    // INLINE OPERATORS
    //-------------------------------------------------------------------

    SLD_API_INLINE_BUFFER
    operator[] (
        u32 index) -> byte& {

        assert(this->is_valid() && index < this->length);
        byte& b = this->data[index];
        return(b);

    };

    SLD_API_INLINE_BUFFER
    operator[] (
        u32 index) const -> const byte& {
            
        assert(this->is_valid() && index < this->length);
        const byte& b = this->data[index];
        return(b);
    } 

};

#endif //SLD_BUFFER_HPP