
#pragma once

#include "sld.hpp"
#include "sld-memory.hpp"
#include "sld-buffer.hpp"
#include "sld-stack.hpp"

#ifndef    SLD_XML_MEMORY_SIZE_KB
#   define SLD_XML_MEMORY_SIZE_KB 512
#endif
#ifndef    SLD_XML_MEMORY_ALLOCATION_GRANULARITY_KB
#   define SLD_XML_MEMORY_ALLOCATION_GRANULARITY_KB 32
#endif


namespace sld {

    using xml_utf8_t = cchar;

    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;

    constexpr u64 XML_MEMORY_SIZE                   = size_kilobytes(SLD_XML_MEMORY_SIZE_KB); 
    constexpr u64 XML_MEMORY_ALLOCATION_GRANULARITY = size_kilobytes(SLD_XML_MEMORY_ALLOCATION_GRANULARITY_KB); 

    SLD_API void          xml_init                      (const u64 memory_size = XML_MEMORY_SIZE, const u64 allocation_granularity = XML_MEMORY_ALLOCATION_GRANULARITY);

    SLD_API xml_doc_t*    xml_doc_create                (void);
    SLD_API xml_doc_t*    xml_doc_destroy               (xml_doc_t* doc);
    SLD_API void          xml_doc_reset                 (xml_doc_t* doc);
    SLD_API u32           xml_doc_buffer_length         (xml_doc_t* doc);
    SLD_API bool          xml_doc_buffer_read           (xml_doc_t* doc, const buffer_t* buffer);
    SLD_API bool          xml_doc_buffer_write          (xml_doc_t* doc, buffer_t*       buffer);
    SLD_API u32           xml_doc_get_child_node_count  (xml_doc_t* doc, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_doc_get_child_node        (xml_doc_t* doc, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_doc_add_child_node        (xml_doc_t* doc, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_doc_get_or_add_child_node (xml_doc_t* doc, const xml_utf8_t* name);

    SLD_API xml_node_t*   xml_node_add_sibling          (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_node_get_next_sibling     (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_node_get_prev_sibling     (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_node_add_child            (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_node_get_child            (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_node_t*   xml_node_get_or_add_child     (xml_node_t* node, const xml_utf8_t* name);
    SLD_API u32           xml_node_get_child_count      (xml_node_t* node, const xml_utf8_t* name);
    SLD_API u32           xml_node_get_attrib_count     (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_attrib_t* xml_node_get_attrib           (xml_node_t* node, const xml_utf8_t* name);
    SLD_API xml_attrib_t* xml_node_add_attrib           (xml_node_t* node, const xml_utf8_t* name);
    
    SLD_API bool          xml_attrib_set_val_utf8       (xml_attrib_t* attrib, const xml_utf8_t* value);
    SLD_API bool          xml_attrib_set_val_u32        (xml_attrib_t* attrib, const u32         value);
    SLD_API bool          xml_attrib_set_val_u64        (xml_attrib_t* attrib, const u64         value);
    SLD_API bool          xml_attrib_set_val_f32        (xml_attrib_t* attrib, const f32         value);
    SLD_API bool          xml_attrib_get_val_utf8       (xml_attrib_t* attrib, xml_utf8_t*&      value);
    SLD_API bool          xml_attrib_get_val_u32        (xml_attrib_t* attrib, const u32&        value);
    SLD_API bool          xml_attrib_get_val_u64        (xml_attrib_t* attrib, const u64&        value);
    SLD_API bool          xml_attrib_get_val_f32        (xml_attrib_t* attrib, const f32&        value);
};
