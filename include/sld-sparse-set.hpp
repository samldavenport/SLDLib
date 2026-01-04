#ifndef SLD_SPARSE_ARRAY_HPP
#define SLD_SPARSE_ARRAY_HPP

#include "sld.hpp"

#define SLD_API_SPARSE_SET_INLINE        template<typename t> inline auto sparse_array<t>::
#define SLD_API_SPARSE_SET_STATIC_INLINE template<typename t> static inline auto sparse_array<t>::
#define SLD_API_SPARSE_SET_CONSTRUCTOR   template<typename t> inline sparse_array<t>::

namespace sld {

    //-------------------------------------------------------------------
    // SPARSE SET
    //-------------------------------------------------------------------

    constexpr u32 SPARSE_ARRAY_INVALID_INDEX    = 0xFFFFFFFF;
    constexpr f32 SPARSE_ARRAY_MAX_LOAD_DEFAULT = 0.7f; 

    template<typename t>
    class sparse_array {

    private:

        // members
        struct {
            u32* key;
            t*   val;
        } _data;
        u32 _capacity;
        u32 _count_max;
        u32 _count_current;

    public:

        // methods
        static inline u32
        calculate_memory_size (
            const u32 capacity) {
            return((sizeof(u32) + sizeof(t)) * capacity);
        }

        inline
        sparse_array(
            const void* memory_start,
            const u32   memory_size,
            const u32   capacity,
            const f32   max_load = SPARSE_ARRAY_MAX_LOAD_DEFAULT
        );
        
        inline void validate              (void)               const;
        inline u32  capacity              (void)               const;
        inline u32  count                 (void)               const;
        inline u32  max_count             (void)               const;
        inline u32  size                  (void)               const;
        inline u32  mask                  (const u32 key)      const;
        inline u32  lookup                (const u32 key)      const;

        inline u32  insert (const u32 key, const t& val);
        inline void remove (const u32 index);

        // operators
        inline t& operator[](u32 index);
    };

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    SLD_API_SPARSE_SET_CONSTRUCTOR
    sparse_array(
        const void* memory_start,
        const u32   memory_size,
        const u32   capacity,
        const f32   max_load_p100) {


        const u32 memory_size_min = sparse_array<t>::calculate_memory_size(capacity);

        bool is_valid = true;
        is_valid &= (memory_size_min != 0);
        is_valid &= (memory_start    != NULL);
        is_valid &= (memory_size     >= memory_size_min);
        is_valid &= (capacity        != 0);
        is_valid &= (max_load_p100   <= 0.0f);
        is_valid &= (max_load_p100   >= 1.0f);
        assert(is_valid);

        this->_data.key      = (u32*)memory_start;
        this->_data.val      = (t*)(((addr)memory_start) + (sizeof(u32) * capacity)); 
        this->_capacity      = capacity;
        this->_count_current = 0;
        this->_count_max     = (u32)((f32)capacity * max_load_p100); 
    }

    SLD_API_SPARSE_SET_INLINE
    validate(
        void) const -> void{

        bool is_valid = true;
        is_valid &= (this->_data.key      != NULL);
        is_valid &= (this->_data.val      != NULL);
        is_valid &= (this->_capacity      != 0);
        is_valid &= (this->_count_max     != 0);
        is_valid &= (this->_count_current <= this->_count_max);
        assert(is_valid);
    }

    SLD_API_SPARSE_SET_INLINE
    capacity(
        void) const -> u32 {

        return(this->_capacity);
    }

    SLD_API_SPARSE_SET_INLINE
    count(
        void) const -> u32 {

        return(this->_count_current);
    }

    SLD_API_SPARSE_SET_INLINE
    max_count(
        void) const -> u32 { 

        return(this->_count_max);
    }

    SLD_API_SPARSE_SET_INLINE
    size(
        void) const -> u32 { 

        const u32 size = (
            (sizeof(u32) + sizeof(t)) * this->_capacity
        );
        return(size);
    }

    SLD_API_SPARSE_SET_INLINE
    mask(
        const u32 key) const -> u32 {
        
        this->validate();
        const u32 mask = (this->_capacity - 1) & key; 
        return(mask);
    }

    SLD_API_SPARSE_SET_INLINE
    lookup(
        const u32 key) const -> u32 {

        this->validate();

        // get sparse index
        u32 index = this->mask(key);
        assert(index < this->_capacity);

        for (
            u32 probe = 0;
                probe < this->_capacity;
              ++probe) {

            // get the sparse data
            const u32 current_key = this->_data.key[index];

            // check if this is the data we need        
            const bool is_empty = (current_key == SPARSE_ARRAY_INVALID_INDEX);
            const bool is_found = (current_key == key);
            
            // these cant be true at the same time
            assert(!(is_empty && is_found)); 

            // if we found the key or the slot is empty, we're done 
            if (is_empty) return(SPARSE_ARRAY_INVALID_INDEX);
            if (is_found) return(index);        

            // not found yet, increase sparse index with wrapping
            index = this->mask(index + 1);
        }

        // we wrapped completely around without finding anything
        // unlikely, but logically possible
        return(SPARSE_ARRAY_INVALID_INDEX);
    }

    SLD_API_SPARSE_SET_INLINE
    insert(
        const u32 key,
        const t&  val) -> u32 {
        
        bool did_insert = false;

        // check args
        this->validate();

        // if we're at max, we're done
        if (this->_count_current == this->_count_max)
            return(did_insert);

        // assert this doesn't exist already
        const bool does_not_exist = (this->lookup(key) == SPARSE_ARRAY_INVALID_INDEX); 
        assert(does_not_exist);

        // get the current sparse index
        u32 index = this->mask(key); 

        for (
            u32 probe = 0;
                probe < this->_capacity;
              ++probe) {

            // if this slot is empty, we're done
            const bool is_empty = (index == SPARSE_ARRAY_INVALID_INDEX);
            if (is_empty) {

                this->_data.val [index] = val;
                this->_data.key [index] = key;
                ++this->_count_current;                
                return(index);
            }

            // not found yet, increase sparse index with wrapping
            index = this->mask(index + 1);
        }

        return(SPARSE_ARRAY_INVALID_INDEX);
    }

    SLD_API_SPARSE_SET_INLINE
    remove(
        const u32 index) -> void {

        this->validate();
        assert(
            index < this->_capacity &&
            this->_data.key[index] != SPARSE_ARRAY_INVALID_INDEX
        );
        this->_data.key[index] = SPARSE_ARRAY_INVALID_INDEX;
    }

    //-------------------------------------------------------------------
    // OPERATORS
    //-------------------------------------------------------------------

    SLD_API_SPARSE_SET_INLINE
    operator[](
        u32 index) -> t& {

        assert(
            index < this->_capacity &&
            this->_data.key[index] != SPARSE_ARRAY_INVALID_INDEX
        );
        return(this->_data.val[index]);
    }
};

#endif //SLD_SPARSE_ARRAY_HPP