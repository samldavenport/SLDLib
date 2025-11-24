#ifndef SLD_ARENA_HPP
#define SLD_ARENA_HPP

#include "sld.hpp"

#define SLD_API_INLINE_ARENA                                          inline auto arena::
#define SLD_API_INLINE_ARENA_TEMPLATE  template<typename struct_type> inline auto arena::

namespace sld {

    //-------------------------------------------------------------------
    // ARENA API
    //-------------------------------------------------------------------

    struct arena {

        // members
        addr start;
        u64  size;
        u64  position;
        u64  save;

        // methods
        inline void  init                (const void* memory, const u64 size);
        inline bool  is_valid            (void);
        inline void  assert_valid        (void);
        inline void  save_position       (void);
        inline void  roll_back           (void);
        inline void  reset               (void);
        inline u64   get_space_remaining (void);
        inline byte* push_bytes          (const u64 size, const u64 alignment = 0);

        // template methods
        template<typename struct_type> inline struct_type* push_struct (const u32 count = 1);
    };

    //-------------------------------------------------------------------
    // ARENA INLINE METHODS
    //-------------------------------------------------------------------


    SLD_API_INLINE_ARENA
    init(
        const void* memory,
        const u64   size) -> void {

        assert(memory != NULL && size != NULL);

        this->start = (addr)memory;
        this->size  = size;

    }

    SLD_API_INLINE_ARENA
    is_valid(
        void) -> bool {

        const bool is_valid = (
            this->start    != NULL      &&
            this->size     != 0         &&
            this->position < this->size &&
            this->save     <= this->position
        );
        return(is_valid);
    }

    SLD_API_INLINE_ARENA
    assert_valid(
        void) -> void {

        assert(this->is_valid());
    }

    SLD_API_INLINE_ARENA
    save_position(
        void) -> void {
        
        this->assert_valid();
        this->save = this->position;
    }

    SLD_API_INLINE_ARENA
    roll_back(
        void) -> void {

        this->assert_valid();
        this->position = this->save;
    }

    SLD_API_INLINE_ARENA
    reset(
        void) -> void {

        this->assert_valid();
        this->position = 0;
        this->save     = 0;
    }

    SLD_API_INLINE_ARENA
    get_space_remaining(
        void) -> u64 {

        this->assert_valid();
        const u64 remaining = (this->size - this->position);
        return(remaining);
    }

    SLD_API_INLINE_ARENA
    push_bytes(
        const u64 size,
        const u64 alignment) -> byte* {

        assert(this->is_valid() && size != 0);

        const u64 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        const u64  new_position = (this->position + size_aligned);
        const bool can_push     = (new_position <= this->size);
        
        byte* bytes = NULL;
        if (can_push) {

            bytes          = (byte*)(this->start + this->position);
            this->position = new_position;
        }
        return(bytes);
    }

    SLD_API_INLINE_ARENA_TEMPLATE 
    push_struct(
        const u32 count) -> struct_type* {

        const u64    size    = count * sizeof(struct_type);
        struct_type* structs = this->push_bytes(size);
        return(structs); 
    }
};

#endif //SLD_ARENA_HPP