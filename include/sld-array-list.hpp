#ifndef SLD_ARRAY_HPP
#define SLD_ARRAY_HPP

#include "sld.hpp"

#define SLD_API_INLINE_ARRAY_LIST template<typename _t> inline auto array_list_t<_t>::

namespace sld {

    //-------------------------------------------------------------------
    // ARRAY LIST
    //-------------------------------------------------------------------


    template<typename _t> 
    struct array_list_t {

        // type alias
        using element_t = _t;

        // members
        element_t* array;
        u32        capacity;
        u32        count;

        // methods
        inline void       init         (element_t* data, const u64 capacity);
        inline bool       is_valid     (void) const;
        inline bool       is_empty     (void) const;
        inline bool       is_full      (void) const;
        inline void       assert_valid (void) const;
        inline element_t& first        (void) const;
        inline element_t& last         (void) const;
        inline void       reset        (void);
        inline bool       add          (const element_t* elment);
        inline bool       add          (const element_t& elment);
        inline bool       insert_at    (const element_t* elment, const u32 index);
        inline bool       insert_at    (const element_t& elment, const u32 index);
        inline void       remove       (const element_t* element);
        inline void       remove       (const element_t& element);
        inline void       remove_at    (const u32  index);

        // operators
        inline element_t&       operator[] (u32 index);
        inline const element_t& operator[] (u32 index) const;
    };

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------
    
    SLD_API_INLINE_ARRAY_LIST
    init(
        element_t*  array,
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
        void) const -> element_t& {

        element_t& elmnt = (!this->is_empty())
            ? this->array[0]
            : NULL;

        return(elmnt);
    }

    SLD_API_INLINE_ARRAY_LIST
    last(
        void) const -> element_t& {

        const u32 index = (this->count - 1);

        element_t& elmnt = (!this->is_empty())
            ? this->array[index]
            : NULL;
        
        return(elmnt);
    }

    SLD_API_INLINE_ARRAY_LIST
    add(
        const element_t* element) -> bool {

        assert(
            this->is_valid() &&
            element != NULL
        );

        const bool can_add = !this->is_full(); 
        if (can_add) {
            const u32 index = this->count;

            auto      dst  = (void*)&this->array[index];
            auto      src  = (void*)element;
            const u32 size = sizeof(element_t);
            memccpy(dst,src, size);
            
            ++this->count;
        } 
        return(can_add);
    }

    SLD_API_INLINE_ARRAY_LIST
    add(
        const element_t& element) -> bool {

        this->assert_valid();

        const bool can_add = !this->is_full(); 
        if (can_add) {
            const u32 index = this->count;

            auto      dst  = (void*)&this->array[index];
            auto      src  = (void*)&element;
            const u32 size = sizeof(element_t);
            memccpy(dst,src, size);
            
            ++this->count;
        } 
        return(can_add);
    }

    SLD_API_INLINE_ARRAY_LIST
    insert_at(
        const element_t* element,
        const u32        index) -> bool {

        assert(
            this->is_valid() &&
            !this->is_empty() &&
            element != NULL  &&
            index   < this->count &&
        );

        const u32  element_size = sizeof(element_t); 
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
            const void* copy_src = (void*)element;
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
        const element_t& element,
        const u32        index) -> bool {

        assert(
            this->is_valid()  &&
            !this->is_empty() &&
            index   < this->count &&
        );

        const u32  element_size = sizeof(element_t); 
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
            const void* copy_src = (void*)&element;
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
        const element_t* element) -> void {

        assert(
            this->is_valid()       &&
            element >= this->array &&
            element <= this->array + this->count
        );

        


    }

    SLD_API_INLINE_ARRAY_LIST
    remove(
        const element_t& element) -> void {
    }

    SLD_API_INLINE_ARRAY_LIST
    remove_at(
        const u32  index) -> void {
    }

    SLD_API_INLINE_ARRAY_LIST
    operator[](
        u32 index) -> element_t& {

        assert(
            this->is_valid()  &&
            !this->is_empty() &&
            this->count > index
        );

        element_t& elmnt = this->array[index];
        return(elmnt); 
    }

    SLD_API_INLINE_ARRAY_LIST
    operator[](
        u32 index) const -> const element_t& {

        assert(
            this->is_valid()  &&
            !this->is_empty() &&
            this->count > index
        );

        const element_t& elmnt = this->array[index];
        return(elmnt); 
    }
};


#endif //SLD_ARRAY_HPP