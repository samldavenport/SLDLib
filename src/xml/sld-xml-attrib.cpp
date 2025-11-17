#pragma once

#include "sld-xml-internal.hpp"

namespace sld {

    SLD_API bool
    xml_attrib_set_val_utf8(
        xml_attrib_t* const attrib,
        const xml_utf8_t*   value) {

        bool can_set = true;
        can_set &= (attrib != NULL);
        can_set &= (value  != NULL);
        assert(can_set);

        const bool is_set = attrib->set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_val_u32(
        xml_attrib_t* const attrib,
        const u32           value) {

        bool can_set = true;
        can_set &= (attrib != NULL);
        can_set &= (value  != NULL);
        assert(can_set);

        const bool is_set = attrib->set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_val_u64(
        xml_attrib_t* const attrib,
        const u64           value) {

        bool can_set = true;
        can_set &= (attrib != NULL);
        can_set &= (value  != NULL);
        assert(can_set);

        const bool is_set = attrib->set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_val_f32(
        xml_attrib_t* const attrib,
        const f32           value) {

        bool can_set = true;
        can_set &= (attrib != NULL);
        can_set &= (value  != NULL);
        assert(can_set);

        const bool is_set = attrib->set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_get_val_utf8(
        xml_attrib_t* const attrib,
        xml_utf8_t*&        value) {

        bool can_get = true;
        can_get &= (attrib != NULL);
        assert(can_get);

        constexpr cchar invalid = 0;
        value = (xml_utf8_t*)attrib->as_string(&invalid);
        const bool did_get = (value[0] != invalid); 
        return(did_get);
    }

    SLD_API bool
    xml_attrib_get_val_u32(
        xml_attrib_t* const attrib,
        u32&                value) {

        bool can_get = true;
        can_get &= (attrib != NULL);
        assert(can_get);

        constexpr u32 invalid = 0xFFFFFFFF;
        value = attrib->as_uint(invalid);

        const bool did_get = (value != invalid); 
        return(did_get);
    }

    SLD_API bool
    xml_attrib_get_val_u64(
        xml_attrib_t* const attrib,
        u64&                value) {

        bool can_get = true;
        can_get &= (attrib != NULL);
        assert(can_get);

        constexpr u64 invalid = 0xFFFFFFFFFFFFFFFF;
        value = attrib->as_ullong(invalid);

        const bool did_get = (value != invalid); 
        return(did_get);
    }

    SLD_API bool
    xml_attrib_get_val_f32(
        xml_attrib_t* const attrib,
        f32&                value) {

        bool can_get = true;
        can_get &= (attrib != NULL);
        assert(can_get);

        constexpr f32 invalid = (f32)(0xFFFFFFFF);
        value = attrib->as_float(invalid);

        const bool did_get = (value != invalid); 
        return(did_get);
    }
};