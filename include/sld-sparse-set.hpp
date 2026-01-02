#ifndef SLD_SPARSE_ARRAY_HPP
#define SLD_SPARSE_ARRAY_HPP

#include "sld.hpp"

#define SLD_API_SPARSE_SET template<typename t> inline auto sparse_array<t>::

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
        inline void
        init(
            const void* memory_start,
            const u32   memory_size,
            const u32   capacity,
            const f32   max_load = SPARSE_ARRAY_MAX_LOAD_DEFAULT
        );

        inline u32  calculate_memory_size (const u32 capacity) const;
        inline void validate              (void)               const;
        inline u32  capacity              (void)               const;
        inline u32  count                 (void)               const;
        inline u32  max_count             (void)               const;
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

    SLD_API_SPARSE_SET
    calculate_memory_size(
        const u32 capacity) const -> u32 {
        
        if (
            capacity == 0    ||
            max_load <= 0.0f ||
            max_load >= 1.0f) {

            return(0);
        }

        const u32 memory_size = (
            (sizeof(u32) + sizeof(t)) * capacity;
        );

        return(memory_size);
    }


    SLD_API_SPARSE_SET
    init(
        const void* memory_start,
        const u32   memory_size,
        const u32   capacity,
        const f32   max_load_p100) -> void {

        const u32 memory_size_min = this->calculate_memory_size(
            capacity,
            max_load);

        bool is_valid = true;
        is_valid &= (memory_size_min != 0);
        is_valid &= (memory_start    != NULL);
        is_valid &= (memory_size     >= memory_size_min);
        is_valid &= (capacity        != 0);
        is_valid &= (max_load_p100   <= 0.0f);
        is_valid &= (max_load_p100   >= 1.0f);
        assert(is_valid);

        this->_data.key      = (*u32)memory_start;
        this->_data.val      = (t*)(((addr)memory_start) + (sizeof(u32) * capacity)); 
        this->_capacity      = capacity;
        this->_count_current = 0;
        this->_count_max     = (u32)((f32)capacity * max_load_p100); 
    }

    SLD_API_SPARSE_SET
    validate(
        void) const -> void{

        bool is_valid = true;
        is_valid &= (this->_data.key      != NULL);
        is_valid &= (this->_data.val.key  != NULL);
        is_valid &= (this->_capacity      != 0);
        is_valid &= (this->_count_max     != 0);
        is_valid &= (this->_count_current <= this->_count_max);
        assert(is_valid);
    }

    SLD_API_SPARSE_SET
    capacity(
        void) const -> u32 {

        return(this->_capacity);
    }

    SLD_API_SPARSE_SET
    count(
        void) const -> u32 {

        return(this->_count);
    }

    SLD_API_SPARSE_SET
    max_count(
        void) const -> u32 { 

        return(this->_count_max);
    }

    SLD_API_SPARSE_SET
    mask(
        const u32 key) const -> u32 {
        
        this->validate();
        const u32 mask = (this->_capacity - 1) & key; 
        return(mask);
    }

    SLD_API_SPARSE_SET
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
            const bool is_empty = (current_key == SPARSE_SET_INVALID_INDEX);
            const bool is_found = (current_key == key);
            
            // these cant be true at the same time
            assert(!(is_empty && is_found)); 

            // if we found the key or the slot is empty, we're done 
            if (is_empty) return(SPARSE_SET_INVALID_INDEX);
            if (is_found) return(index);        

            // not found yet, increase sparse index with wrapping
            index = this->mask(index + 1);
        }

        // we wrapped completely around without finding anything
        // unlikely, but logically possible
        return(SPARSE_SET_INVALID_INDEX);
    }

    SLD_API_SPARSE_SET
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
        const bool does_not_exist = (this->lookup(key) == SPARSE_SET_INVALID_INDEX); 
        assert(does_not_exist);

        // get the current sparse index
        u32 index = this->mask(key); 

        for (
            u32 probe = 0;
                probe < this->_capacity;
              ++probe) {

            // if this slot is empty, we're done
            const bool is_empty = (dense_index == SPARSE_SET_INVALID_INDEX);
            if (is_empty) {


                this->_data.val [index] = val;
                this->_data.key [index] = key;
                ++this->_count_current;                
                return(dense_index);
            } 
        }

        return(SPARSE_SET_INVALID_INDEX);
    }

    SLD_API_SPARSE_SET
    remove(
        const u32 index) -> void {

        this->validate();
        assert(
            index < this->capacity &&
            this->_data.key[index] != SPARSE_SET_INVALID_INDEX;
        );
        this->_data.key[index] = SPARSE_SET_INVALID_INDEX;
    }

    //-------------------------------------------------------------------
    // OPERATORS
    //-------------------------------------------------------------------

    SLD_API_SPARSE_SET
    operator[](
        u32 index) -> t& {

        assert(
            index < this->capacity &&
            this->_data.key[index] != SPARSE_SET_INVALID_INDEX;
        );
        return(this->_data.val);
    }
};

#endif //SLD_SPARSE_ARRAY_HPP