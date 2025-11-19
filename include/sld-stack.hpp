#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"
#include "sld-arena.hpp"

namespace sld {


    //-------------------------------------------------------------------
    // STACK API
    //-------------------------------------------------------------------

    constexpr u64 STACK_DEFAULT_ALIGNMENT = 4;

    struct stack_t {
        byte* data;
        u64   capacity;
        u64   position;
        u64   save;
    };

    SLD_API_INLINE void  stack_init     (stack_t* stack, byte* array,    const u64 capacity);
    SLD_API_INLINE byte* stack_push     (stack_t* stack, const u64 size, const u64 alignment = STACK_DEFAULT_ALIGNMENT);
    SLD_API_INLINE bool  stack_pull     (stack_t* stack, const u64 size, const u64 alignment = STACK_DEFAULT_ALIGNMENT);
    SLD_API_INLINE bool  stack_is_valid (stack_t* stack);
    SLD_API_INLINE void  stack_assert   (stack_t* stack);
    SLD_API_INLINE void  stack_reset    (stack_t* stack);
    SLD_API_INLINE void  stack_revert   (stack_t* stack);
    SLD_API_INLINE void  stack_save     (stack_t* stack);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    stack_init(
        stack_t*  stack,
        byte*     data,
        const u64 capacity) {
        
        assert(
            stack    != NULL &&
            data     != NULL &&
            capacity != 0
        );

        memset(data, 0, capacity);

        stack->data     = data; 
        stack->capacity = capacity;
        stack->position = 0;
        stack->save     = 0;

        stack_assert(stack);
    }

    SLD_API_INLINE bool
    stack_is_valid(stack_t* stack)  { 

        assert(stack);

        const bool is_valid = (
            (stack->data     != NULL)            && 
            (stack->capacity != 0)               && 
            (stack->position <  stack->capacity) && 
            (stack->save     <= stack->position)
        );
        return(is_valid);
    }

    SLD_API_INLINE void
    stack_assert(stack_t* stack) {
        assert(stack_is_valid(stack));
    }

    SLD_API_INLINE void 
    stack_reset(
        stack_t* stack) {

        stack_assert(stack);

        memset(stack->data, 0, stack->capacity);

        stack->position = 0;
        stack->save     = 0;
    }

    SLD_API_INLINE void
    stack_revert(stack_t* stack) {

        stack_assert(stack);
        stack->position = stack->save;
    }

    SLD_API_INLINE void
    stack_save(stack_t* stack) {
        
        stack_assert(stack);
        stack->save = stack->position;
    }

    SLD_API_INLINE byte*
    stack_push(
        stack_t*  stack,
        const u64 size,
        const u64 alignment) {

        stack_assert(stack);
        assert(size != 0);

        const u64 align_pow_2  = size_round_up_pow2 (alignment);
        const u64 size_aligned = size_align_pow_2   (size, align_pow_2); 
        const u64 new_position = stack->position + size_aligned;
        
        bool can_push = (new_position <= stack->capacity);
        if (!can_push) return(NULL);

        byte* ptr = &stack->data[stack->position];
        stack->position = new_position;

        return(ptr);
    }

    SLD_API_INLINE bool
    stack_pull(
        stack_t*  stack,
        const u64 size,
        const u64 alignment) {

        stack_assert(stack);
        assert(size != 0);

        const u64 align_pow_2  = size_round_up_pow2 (alignment);
        const u64 size_aligned = size_align_pow_2   (size, align_pow_2); 
        const bool can_pull    = (size_aligned <= stack->position);
        
        if (can_pull) {
            stack->position -= size_aligned;
            if (stack->save > stack->position) {
                stack->save = 0;
            }
        }
        
        return(can_pull);
    }
};

#endif //SLD_STACK_HPP