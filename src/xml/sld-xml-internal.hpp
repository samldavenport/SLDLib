#ifndef SLD_XML_INTERNAL_HPP
#define SLD_XML_INTERNAL_HPP

#include <pugixml.hpp>
#include "sld.hpp"
#include "sld-xml.hpp"
#include "sld-memory.hpp"
#include "sld-block-allocator.hpp"


namespace sld {

    using xml_block_allocator_t = block_allocator_t;
    using xml_pugi_memory_t     = void;

    SLD_INTERNAL void               xml_allocator_reserve_os_memory (const u32 size_total, const u32 size_block);
    SLD_INTERNAL void               xml_allocator_release_os_memory (void);
    SLD_INTERNAL xml_doc_t*         xml_allocator_commit_doc        (void);
    SLD_INTERNAL xml_pugi_memory_t* xml_allocator_commit_pugi       (const u32 pugi_size);
    SLD_INTERNAL void               xml_allocator_decommit_doc      (xml_doc_t* xml_doc);
    SLD_INTERNAL void               xml_allocator_decommit_pugi     (xml_pugi_memory_t* pugi);

    SLD_INTERNAL xml_node_t*        xml_doc_push_node               (xml_doc_t* xml_doc);
    SLD_INTERNAL xml_attrib_t*      xml_doc_push_attrib             (xml_doc_t* xml_doc);

    struct xml_doc_t : pugi::xml_document {
        stack stack; 
    };     
    
    struct xml_node_t : pugi::xml_node { 
        xml_doc_t* doc;
    };      

    struct xml_attrib_t : pugi::xml_attribute { 
        xml_node_t* node;        
        xml_doc_t*  doc;
    };      

    struct xml_writer_t : pugi::xml_writer {

        buffer buffer;

        xml_writer_t() {
            buffer.data   = NULL;
            buffer.size   = 0;
            buffer.length = 0;
        }

        void
        set_buffer_memory(
            byte* data,
            u64   size) {

            buffer.data   = data;
            buffer.size   = size;
            buffer.length = 0;
        }

        void
        clear_buffer_memory(
            void) {

            buffer.data   = NULL;
            buffer.size   = 0;
            buffer.length = 0;
        }

        virtual void
        write(
            const void* data,
            size_t      size) {

            // set the length
            buffer.length = size;

            // check if we can copy the buffer
            // if not, return
            bool can_write = true;
            can_write &= (data          != NULL);
            can_write &= (buffer.length != 0);
            can_write &= (buffer.length <= buffer.size);
            if (!can_write) return;

            // copy the buffer
            (void)buffer_copy(&buffer, (byte*)data, size);
        }
    };
};

#endif //SLD_XML_INTERNAL_HPP