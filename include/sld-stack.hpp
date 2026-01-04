#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"

#define SLD_API_INLINE_STACK          inline auto stack::
#define SLD_API_INLINE_STACK_TEMPLATE template<typename struct_type> inline auto stack::

namespace sld {

    //-------------------------------------------------------------------
    // STACK
    //-------------------------------------------------------------------

    constexpr u32 STACK_DEFAULT_ALIGNMENT = 4;

    struct stack {

        // members        
        byte* data;
        u32   capacity;
        u32   position;
        u32   save;

        // constructor
        explicit inline
        stack(
            byte*     data,
            const u32 capacity);

        // methods
        inline void  init          (byte* data,     const u32 capacity);
        inline byte* push          (const u32 size, const u32 alignment = STACK_DEFAULT_ALIGNMENT);
        inline bool  pull          (const u32 size, const u32 alignment = STACK_DEFAULT_ALIGNMENT);
        inline bool  is_valid      (void) const;
        inline void  assert_valid  (void) const;
        inline void  reset         (void);
        inline void  roll_back     (void);
        inline void  save_position (void);

        // template methods
        template<typename struct_type> inline struct_type* push_struct (const u32 count = 1);
        template<typename struct_type> inline bool         pull_struct (const u32 count = 1);
    };

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    inline
    stack::stack(
        byte*     data,
        const u32 capacity) {

        assert(
            data     != NULL &&
            capacity != 0
        );

        this->data     = data;
        this->capacity = capacity;
        this->position = 0;
        this->save     = 0;
    }

    // methods
    SLD_API_INLINE_STACK
    init(
        byte*     data,
        const u32 capacity) -> void {

        assert(
            data     != NULL &&
            capacity != 0
        );

        this->data     = data;
        this->capacity = capacity;
        this->position = 0;
        this->save     = 0;
    }

    SLD_API_INLINE_STACK
    push(
        const u32 size,
        const u32 alignment) -> byte* {

        assert(
            this->is_valid() &&
            size != 0
        );

        const u32 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        const u32 new_position = (this->position + size_aligned);

        byte* push_data = NULL;
        if (new_position <= this->capacity) {
            push_data = &this->data[this->position];
            this->position = new_position;
        }
        return(push_data);
    }

    SLD_API_INLINE_STACK
    pull(
        const u32 size,
        const u32 alignment) -> bool {

        assert(
            this->is_valid() &&
            size != 0
        );

        const u32 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        const bool can_pull = (size_aligned <= this->position); 
        if (can_pull) {
            this->position -= size_aligned;
            if (this->position < this->save) {
                this->save = 0;
            }
        };
        return(can_pull);
    }

    SLD_API_INLINE_STACK
    is_valid(
        void) const -> bool {

        const bool is_valid = (
            (this->data     != 0)              &&
            (this->capacity != 0)              &&
            (this->position <= this->capacity) &&
            (this->save     <= this->position)
        );
        return(is_valid);
    }

    SLD_API_INLINE_STACK
    assert_valid(
        void) const -> void {

        assert(this->is_valid());
    }
    
    SLD_API_INLINE_STACK
    reset(
        void) -> void {

        this->assert_valid();
        this->position = 0;
        this->save     = 0;
    }

    SLD_API_INLINE_STACK
    roll_back(
        void) -> void {

        this->assert_valid();
        this->position = this->save;
    }
    
    SLD_API_INLINE_STACK
    save_position(
        void) -> void {

        this->assert_valid();
        this->save = this->position;
    }

    // template methods
    SLD_API_INLINE_STACK_TEMPLATE
    push_struct(
        const u32 count) -> struct_type* {

        const u32    struct_size = sizeof(struct_type) * count;
        struct_type* struct_inst = (struct_type*)this->push(struct_size);
        return(struct_inst);
    }
    
    SLD_API_INLINE_STACK_TEMPLATE
    pull_struct(
        const u32 count) -> bool {

        const u32  struct_size = sizeof(struct_type) * count;
        const bool did_pull    = this->pull(struct_size); 
        return(did_pull);
    }
};

#endif //SLD_STACK_HPP