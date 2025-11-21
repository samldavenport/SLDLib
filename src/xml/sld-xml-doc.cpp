#pragma once

#include "sld-xml-internal.hpp"

namespace sld {

    SLD_API void
    xml_doc_reset(
        xml_doc_t* const doc) {

        doc->reset();
    }

    SLD_API bool
    xml_doc_buffer_read(
        xml_doc_t* const     doc,
        const buffer_t* buffer) {

        const u64 buffer_length = xml_doc_buffer_length(doc);

        assert(doc);

        const pugi::xml_parse_result result = doc->load_buffer(
            (void*)buffer->data,
            buffer->size
        );

        const bool did_read = (result.status == pugi::xml_parse_status::status_ok);
        return(did_read);
    }

    SLD_API bool
    xml_doc_buffer_write(
        xml_doc_t* const doc,
        buffer_t*   buffer) {

        assert(doc);

        xml_writer_t writer;
        writer.buffer.data   = buffer->data;
        writer.buffer.size   = buffer->size;
        writer.buffer.length = buffer->length;

        doc->save(writer);

        buffer->data   = writer.buffer.data;
        buffer->size   = writer.buffer.size;
        buffer->length = writer.buffer.length;

        const bool did_write = (buffer->length != 0);
        return(did_write);
    }
    
    SLD_API u32
    xml_doc_buffer_length(
        xml_doc_t* const doc) {

        xml_writer_t xml_writer;
        xml_writer.buffer.data = NULL;

        doc->save(xml_writer);

        const u32 length = xml_writer.buffer.length;
        return(length);
    }

    SLD_API xml_node_t* 
    xml_doc_get_child_node(
        xml_doc_t* const doc,
        const xml_utf8_t* child_name) {

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node  = doc->child(child_name); 
        xml_node_t*    child_node = xml_doc_push_node(doc);
        *child_node = *((xml_node_t*)&pugi_node);
        child_node->doc = doc;

        return(child_node);
    }

    SLD_API xml_node_t*
    xml_doc_get_or_add_child_node(
        xml_doc_t* const  doc,
        const xml_utf8_t* child_name) {

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        assert(is_valid);

        xml_node_t*    child_node = xml_doc_push_node(doc);
        pugi::xml_node pugi_node  = doc->child(child_name);
        if (pugi_node == NULL) {
            pugi_node = doc->append_child(child_name);            
        }
        *child_node = *((xml_node_t*)&pugi_node);
        return(child_node);
    }

    SLD_API xml_node_t*
    xml_doc_add_child_node(
        xml_doc_t* const  doc,
        const xml_utf8_t* child_name) {

        bool is_valid = true;
        is_valid &= (doc        != NULL);
        is_valid &= (child_name != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node  = doc->append_child(child_name);
        xml_node_t*    child_node = xml_doc_push_node(doc); 
        *child_node = *((xml_node_t*)&pugi_node);
        return(child_node);
    }

    SLD_API u32
    xml_doc_get_child_node_count(
        xml_doc_t* const  doc,
        const xml_utf8_t* child_name) {

        assert(doc);
        assert(child_name);

        u32  count    = 0;
        auto children = doc->children(child_name); 
        for (
            auto iter = children.begin();
            iter     != children.end();
            ++iter) {
            ++count;
        }
    
        return(count);
    }
    
    SLD_INTERNAL xml_node_t*
    xml_doc_push_node(
        xml_doc_t* xml_doc) {

        auto node = xml_doc->stack.push_struct<xml_node_t>();
        assert(node);
        return(node);
    }

    SLD_INTERNAL xml_attrib_t*
    xml_doc_push_attrib(
        xml_doc_t* xml_doc) {

        auto attrib = xml_doc->stack.push_struct<xml_attrib_t>();
        assert(attrib);
        return(attrib);
    }
};