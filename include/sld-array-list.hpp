#ifndef SLD_ARRAY_LIST_HPP
#define SLD_ARRAY_LIST_HPP

#include "sld.hpp"

#define SLD_API_INLINE_ARRAY_LIST template<typename t> inline auto array_list<t>::

namespace sld {

    //-------------------------------------------------------------------
    // ARRAY LIST
    //-------------------------------------------------------------------

    constexpr u32 ARRAY_LIST_INVALID_INDEX = 0xFFFFFFFF;

    template<typename t> 
    struct array_list {

        // type alias
        using element = t;

        // members
        element* array;
        u32      capacity;
        u32      count;

        // methods
        inline void     init         (element* data, const u64 capacity);
        inline void     validate     (void) const;
        inline bool     is_valid     (void) const;
        inline bool     is_empty     (void) const;
        inline bool     is_full      (void) const;
        inline void     assert_valid (void) const;
        inline element& first        (void) const;
        inline element& last         (void) const;
        inline void     reset        (void);
        inline bool     add          (const element* elmnt);
        inline bool     add          (const element& elmnt);
        inline bool     insert_at    (const element* elmnt, const u32 index);
        inline bool     insert_at    (const element& elmnt, const u32 index);
        inline void     remove       (const element* elmnt);
        inline void     remove       (const element& elmnt);
        inline void     remove_at    (const u32 index);
        inline u32      index_of     (const element* elmnt) const;
        inline u32      index_of     (const element& elmnt) const;

        // operators
        inline element&       operator[] (u32 index);
        inline const element& operator[] (u32 index) const;
    };

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------
    
    SLD_API_INLINE_ARRAY_LIST
    init(
        element*  array,
        const u64 capacity) -> void {

        assert(
            array    != NULL &&
            capacity != 0
        );

        this->array    = array;
        this->capacity = capacity;
        this->count    = 0;
    }

    SLD_API_INLINE_ARRAY_LIST
    validate(
        void) const -> void {

        assert(this->is_valid());
    }

    SLD_API_INLINE_ARRAY_LIST
    is_valid(
        void) const -> bool {

        const bool is_valid = (
            (this->array    != NULL)    &&
            (this->capacity != 0)       &&
            (this->count    <= this->capacity)
        );
        return(is_valid);
    }

    SLD_API_INLINE_ARRAY_LIST
    is_empty(
        void) const -> bool {

        this->assert_valid();
        return(this->count == 0);
    }
    SLD_API_INLINE_ARRAY_LIST
    is_full(
        void) const -> bool {

        this->assert_valid();
        return(this->count == this->capacity);
    }

    SLD_API_INLINE_ARRAY_LIST
    assert_valid(
        void) const -> void {
        
        assert(this->is_valid());
    }

    SLD_API_INLINE_ARRAY_LIST
    reset(
        void) -> void {

        this->assert_valid();
        this->count = 0;
    }

    SLD_API_INLINE_ARRAY_LIST
    first(
        void) const -> element& {

        element& elmnt = (!this->is_empty())
            ? this->array[0]
            : NULL;

        return(elmnt);
    }

    SLD_API_INLINE_ARRAY_LIST
    last(
        void) const -> element& {

        const u32 index = (this->count - 1);

        element& elmnt = (!this->is_empty())
            ? this->array[index]
            : NULL;
        
        return(elmnt);
    }

    SLD_API_INLINE_ARRAY_LIST
    add(
        const element* element) -> bool {

        assert(
            this->is_valid() &&
            element != NULL
        );

        const bool can_add = !this->is_full(); 
        if (can_add) {
            const u32 index = this->count;

            auto      dst  = (void*)&this->array[index];
            auto      src  = (void*)element;
            const u32 size = sizeof(element);
            memccpy(dst,src, size);
            
            ++this->count;
        } 
        return(can_add);
    }

    SLD_API_INLINE_ARRAY_LIST
    add(
        const element& element) -> bool {

        this->assert_valid();

        const bool can_add = !this->is_full(); 
        if (can_add) {
            const u32 index = this->count;

            auto      dst  = (void*)&this->array[index];
            auto      src  = (void*)&element;
            const u32 size = sizeof(element);
            memcpy(dst,src, size);
            
            ++this->count;
        } 
        return(can_add);
    }

    SLD_API_INLINE_ARRAY_LIST
    insert_at(
        const element* elmnt,
        const u32      index) -> bool {

        assert(
            this->is_valid() &&
            !this->is_empty() &&
            elmnt != NULL  &&
            index   < this->count &&
        );

        const u32  element_size = sizeof(element); 
        const bool can_add      = !this->is_full();
        if (can_add) {
            
            if (index > 0) {
                void*       move_dst  = (void*)&this->array[index + 1];
                const void* move_src  = (void*)&this->array[index];
                const u32   move_size = element_size * (this->count - index);
                memmove(
                    move_dst
                    move_src
                    move_size
                );
            } 

            void*       copy_dst = (void*)&this->array[index];
            const void* copy_src = (void*)elmnt;
            memccpy(
                copy_dst,
                copy_src,
                element_size
            );
            ++this->count;
        }

        return(can_add);
    }

    SLD_API_INLINE_ARRAY_LIST
    insert_at(
        const element& elmnt,
        const u32      index) -> bool {

        assert(
            this->is_valid()  &&
            !this->is_empty() &&
            index < this->count
        );

        const u32  element_size = sizeof(element); 
        const bool can_add      = !this->is_full();
        if (can_add) {
            
            if (index > 0) {
                void*       move_dst  = (void*)&this->array[index + 1];
                const void* move_src  = (void*)&this->array[index];
                const u32   move_size = element_size * (this->count - index);
                memmove(
                    move_dst,
                    move_src,
                    move_size
                );
            } 

            void*       copy_dst = (void*)&this->array[index];
            const void* copy_src = (void*)&elmnt;
            memccpy(
                copy_dst,
                copy_src,
                element_size
            );
            ++this->count;
        }

        return(can_add);
    }

    SLD_API_INLINE_ARRAY_LIST
    remove(
        const element& elmnt) -> void {

        const u32 index = this->index_of(elmnt);
        assert(
            index != ARRAY_LIST_INVALID_INDEX &&
            index <  this->count
        );
        this->remove_at(index);

    }

    SLD_API_INLINE_ARRAY_LIST
    remove(
        const element* elmnt) -> void {
    
        const u32 index = this->index_of(elmnt);
        assert(
            index != ARRAY_LIST_INVALID_INDEX &&
            index <  this->count
        );
        this->remove_at(index);
    }

    SLD_API_INLINE_ARRAY_LIST
    remove_at(
        const u32 index) -> void {
    
        assert(
            this->is_valid() &&
            index != ARRAY_LIST_INVALID_INDEX &&
            index <  this->count;
        );
    
        if (index < (this->count - 1)) {
            void*       dst  = (void*)&this->array[index];
            const void* src  = (void*)&this->array[index + 1];
            const u32   size = (this->count - index) * sizeof(element); 
            memmove(dst, src, size);
        }
        --this->count;
    }

    SLD_API_INLINE_ARRAY_LIST
    index_of(
        const element* elmnt) const -> u32 {

        assert(
            this->is_valid() &&
            elmnt != NULL
        );

        const intptr_t  start      = this->array; 
        const ptrdiff_t diff       = elmnt - this->array; 
        const u32       stride     = sizeof(elmnt);
        const bool      does_exist = (
            elmnt >= this->array                    &&
            elmnt <= (this->array + this->capacity) &&
            (diff % stride) == 0
        );
        const u32 index = does_exist
            ? (diff / stride)
            : ARRAY_LIST_INVALID_INDEX;

        return(index);
    }

    SLD_API_INLINE_ARRAY_LIST
    index_of(
        const element& elmnt) const -> u32 {

        this->assert_valid();

        u32 index = ARRAY_LIST_INVALID_INDEX;

        const element* tmp_array = this->array;

        for (
            u32 i = 0;
                i < this->count;
              ++i) {

            if (tmp_array[i] == elmnt) {
                index = i;
            }
        }
        return(index);
    }

    SLD_API_INLINE_ARRAY_LIST
    operator[](
        u32 index) -> element& {

        assert(
            this->is_valid()  &&
            !this->is_empty() &&
            this->count > index
        );

        element& elmnt = this->array[index];
        return(elmnt); 
    }

    SLD_API_INLINE_ARRAY_LIST
    operator[](
        u32 index) const -> const element& {

        assert(
            this->is_valid()  &&
            !this->is_empty() &&
            this->count > index
        );

        const element& elmnt = this->array[index];
        return(elmnt); 
    }
};


#endif //SLD_ARRAY_LIST_HPP