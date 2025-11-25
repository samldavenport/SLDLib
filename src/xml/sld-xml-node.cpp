#pragma once

#include "sld-xml-internal.hpp"
#include <meow-hash/meow_hash_x64_aesni.h>

namespace sld {

    constexpr u32 _node_name_length_max = 256;

    SLD_API xml_node_t* 
    xml_node_add_sibling(
        xml_node_t*       node,
        const xml_utf8_t* sibling_name) {

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 

        xml_node_t*    sibling           = xml_doc_push_node(node->doc);
        pugi::xml_node pugi_node_parent  = node->parent();
        pugi::xml_node pugi_node_sibling = pugi_node_parent.append_child(sibling_name);
        *sibling = *(xml_node_t*)&pugi_node_sibling; 
        sibling->doc = node->doc;
        return(sibling);
    }

    SLD_API xml_node_t*
    xml_node_get_next_sibling(
        xml_node_t*       node,
        const xml_utf8_t* sibling_name) {
   
        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 

        xml_node_t*    sibling           = xml_doc_push_node  (node->doc);        
        pugi::xml_node pugi_node_sibling = node->next_sibling (sibling_name);
        *sibling = *(xml_node_t*)&pugi_node_sibling; 
        sibling->doc = node->doc;
        return(sibling);
    }

    SLD_API xml_node_t*
    xml_node_get_prev_sibling(
        const xml_node_t* node,
        const xml_utf8_t* sibling_name) {
   
        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 

        xml_node_t*    sibling           = xml_doc_push_node  (node->doc);        
        pugi::xml_node pugi_node_sibling = sibling->previous_sibling(sibling_name);
        *sibling = *(xml_node_t*)&pugi_node_sibling; 
        sibling->doc = node->doc;
        return(sibling);
    }

    SLD_API xml_node_t* 
    xml_node_add_child(
        xml_node_t* const node,
        const xml_utf8_t* name) {
   
        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        assert(is_valid);

        xml_node_t*    child     = xml_doc_push_node(node->doc);
        pugi::xml_node pugi_node = node->append_child(name);
        *child = *(xml_node_t*)&pugi_node; 
        child->doc = node->doc;
        return(child);
    }

    SLD_API xml_node_t* 
    xml_node_get_child(
        xml_node_t* const node,
        const xml_utf8_t* name) {
   
        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        assert(is_valid);

        xml_node_t*    child     = xml_doc_push_node(node->doc);
        pugi::xml_node pugi_node = node->child(name);
        *child = *(xml_node_t*)&pugi_node; 
        return(child);
    }

    SLD_API xml_node_t*
    xml_node_get_or_add_child(
        xml_node_t*       node,
        const xml_utf8_t* name) {

        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        assert(is_valid);

        xml_node_t*    child_node = xml_doc_push_node(node->doc);
        pugi::xml_node pugi_node  = node->child(name);
        if (pugi_node == NULL) {
            pugi_node = node->append_child(name);            
        }
        *child_node = *((xml_node_t*)&pugi_node);
        return(child_node);
    }

    SLD_API u32
    xml_node_get_child_count(
        xml_node_t* const node,
        const xml_utf8_t* child_name) {

        auto children = node->children(child_name);

        u32 count = 0;
        for (
            auto iter = children.begin();
            iter != children.end();
            ++iter) {
            ++count;
        }
        return(count);
    }

    SLD_API u32
    xml_node_get_attrib_count(
        xml_node_t*       node,
        const xml_utf8_t* name) {

        auto attribs = node->attributes();

        u32 count = 0;
        for (
            auto iter = attribs.begin();
            iter != attribs.end();
            ++iter) {
            ++count;
        }
        return(count);        
    }

    SLD_API xml_attrib_t*
    xml_node_get_attrib(
        xml_node_t*       node,
        const xml_utf8_t* name) {
   
        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        assert(is_valid);

        xml_attrib_t*       attrib      = xml_doc_push_attrib(node->doc);
        pugi::xml_attribute pugi_attrib = node->attribute(name);
        *attrib = *(xml_attrib_t*)&pugi_attrib; 
        return(attrib);
    }

    SLD_API xml_attrib_t*
    xml_node_add_attrib(
        xml_node_t*       node,
        const xml_utf8_t* name) {

        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        assert(is_valid);

        xml_attrib_t*       attrib      = xml_doc_push_attrib(node->doc);
        pugi::xml_attribute pugi_attrib = node->append_attribute(name);
        *attrib = *(xml_attrib_t*)&pugi_attrib; 
        attrib->doc  = node->doc;
        attrib->node = node;
        return(attrib);
    }
};