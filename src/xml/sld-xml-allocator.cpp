#pragma once

#include "sld-xml-internal.hpp"

namespace sld {

    static xml_block_allocator_t _xml_allocator;

    SLD_INTERNAL void
    xml_allocator_reserve_os_memory(
        const u32 size_total, 
        const u32 size_block) {

        block_allocator_reserve_os_memory(&_xml_allocator, size_total, size_block);
    }

    SLD_INTERNAL void
    xml_allocator_release_os_memory(
        void) {

        block_allocator_release_os_memory(&_xml_allocator);
    }

    SLD_INTERNAL xml_doc_t*
    xml_allocator_commit_doc(
        void) {

        constexpr u32 doc_header_size = sizeof(xml_doc_t);

        // commit memory        
        memory_t memory_doc   = block_allocator_commit_memory (&_xml_allocator);
        memory_t memory_stack = memory_add_offset             (memory_doc, doc_header_size);

        // initialize the doc
        xml_doc_t* doc = new (memory_doc.ptr) xml_doc_t();
        doc->stack.init(memory_stack.bytes, memory_stack.size);

        return(doc);
    }

    SLD_INTERNAL xml_pugi_memory_t*
    xml_allocator_commit_pugi(
        const u32 size) {

        xml_pugi_memory_t* pugi = block_allocator_commit(&_xml_allocator);
        assert(size <= _xml_allocator.block_size);
        assert(pugi);        
        return(pugi);
    }

    SLD_INTERNAL void
    xml_allocator_decommit_doc(
        xml_doc_t* xml_doc) {

        block_allocator_decommit(&_xml_allocator, (void*)xml_doc);
    }

    SLD_INTERNAL void
    xml_allocator_decommit_pugi(
        xml_pugi_memory_t* pugi) {
        
        block_allocator_decommit(&_xml_allocator, pugi);
    }
};