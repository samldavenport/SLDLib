#ifndef SLD_ARENA_HPP
#define SLD_ARENA_HPP

#include "sld-memory.hpp"

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
        inline bool  can_push_bytes      (const u64 size);
        inline byte* push_bytes          (const u64 size);

        // template methods
        inline template<typename struct_type> bool         can_push_struct (const u32 count = 1);
        inline template<typename struct_type> struct_type* push_struct     (const u32 count = 1);
    };

    //-------------------------------------------------------------------
    // ARENA INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE bool
    arena_is_valid(
        const arena_t* arena) {

        bool is_valid = (arena != NULL);
        if (is_valid) {

            is_valid &= (arena->size     != 0); 
            is_valid &= (arena->position <  arena->size); 
            is_valid &= (arena->save     <= arena->position); 
        }
        return(is_valid);
    }

    SLD_API_INLINE void
    arena_assert_valid(
        const arena_t* arena) {

        assert(arena_is_valid(arena));
    }

    SLD_API_INLINE void*
    arena_get_header(
        const arena_t* arena) {

        arena_assert_valid(arena);
        void* arena_header = (void*)arena;
        return(arena_header);
    }

    SLD_API_INLINE u64
    arena_get_space_remaining(
        const arena_t* arena) {

        arena_assert_valid(arena);

        const u64 space_remaining = (arena->size - arena->position);
        return(space_remaining);
    }

    SLD_API_INLINE bool
    arena_can_push(
        const arena_t* arena,
        const u64      size,
        const u64      alignment) {

        arena_assert_valid(arena);

        const u64 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        const u64  size_remaining = arena_get_space_remaining(arena);
        const bool can_push       = size_aligned <= size_remaining;
        return(can_push); 
    }

    SLD_API_INLINE void
    arena_reset(
        arena_t* arena) {

        arena_assert_valid(arena);
        arena->position = 0;
        arena->save     = 0;
    }

    SLD_API_INLINE void
    arena_save(
        arena_t* arena) {

        arena_assert_valid(arena);
        arena->save = arena->position;
    }

    SLD_API_INLINE void
    arena_roll_back(
        arena_t* arena) {

        arena_assert_valid(arena);
        arena->position = arena->save;
    }

    SLD_API_INLINE void*
    arena_push_bytes(
        arena_t* arena,
        const u64       size,
        const u64       alignment) {

        arena_assert_valid(arena);

        const u64 alignment_pow_2 = size_round_up_pow2 (alignment);
        const u64 size_aligned    = size_align_pow_2   (size, alignment);
        const u64 new_position    = arena->position + size_aligned;
        
        void* ptr = NULL;
        if (new_position <= arena->size) {

            ptr = arena_get_position(arena);
            assert(ptr);
            
            arena->position = new_position;
        } 
        return(ptr);
    }
    
    SLD_API_INLINE_TEMPLATE type*
    arena_push_struct(
        arena_t*  arena,
        const u32 count) {

        arena_assert_valid(arena);
        assert(count != 0);

        const u64 struct_alignment  = alignof (type);
        const u64 struct_size       = sizeof  (type);
        const u64 struct_array_size = (struct_size * count); 
         
        type* ptr = (type*)arena_push_bytes(
            arena,
            struct_array_size,
            struct_alignment);

        return(ptr);
    }

};

#endif //SLD_ARENA_HPP