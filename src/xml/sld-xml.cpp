#pragma once

#include "sld-xml-internal.hpp"
#include "sld-xml-allocator.cpp"
#include "sld-xml-attrib.cpp"
#include "sld-xml-doc.cpp"
#include "sld-xml-node.cpp"

namespace sld {

    SLD_API void
    xml_init(
        const u64 memory_size,
        const u64 allocation_granularity) {

        xml_allocator_reserve_os_memory(
            memory_size,
            allocation_granularity
        );
    }

};

