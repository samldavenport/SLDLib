#ifndef SLD_SPARSE_ARRAY_HPP
#define SLD_SPARSE_ARRAY_HPP

#include "sld.hpp"

namespace sld {

    template<typename t>
    struct sparse_data {
        t*  value;
        u32 key;
        u32 index;
    };

    template<typename t>
    class sparse_array {

    private:

        struct {
            u32* key;
            t*   val;
        } _data;

        u32 _capacity;
        u32 _count_max;
        u32 _count_current;

    public:

        inline void
        init(
            const void* memory_start,
            const u32   memory_size,
            const f32   max_load = 0.7f
        );

        inline void validate  (void) const;
        inline u32  mask      (void) const;
        inline bool lookup    (const u32 key, u32& index);
        inline bool insert    (const u32 key, t* val);
        inline bool insert    (const u32 key, t& val);
        inline void remove_at (const u32 index);
        inline void update_at (const u32 index);

        inline sparse_data<t> operator[](u32 index);
    };
};

#endif //SLD_SPARSE_ARRAY_HPP