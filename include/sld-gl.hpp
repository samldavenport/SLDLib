#ifndef SLD_GL_SHADER_HPP
#define SLD_GL_SHADER_HPP

#include "sld.hpp"
#include "sld-vector.hpp"
#include "sld-matrix.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------
    
    using gl_error        = GLint;
    using gl_uniform      = GLint;
    using gl_status       = GLint; 
    using gl_shader       = GLuint;
    using gl_shader_stage = GLuint;
    using gl_vertex       = GLuint; 
    using gl_attribute    = GLuint; 
    using gl_buffer       = GLuint;

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr gl_error        GL_ERROR_SUCCESS        = 0; 
    constexpr gl_shader       GL_SHADER_INVALID       = 0xFFFFFFFF;
    constexpr gl_shader_stage GL_SHADER_STAGE_INVALID = 0xFFFFFFFF;
    constexpr gl_uniform      GL_UNIFORM_INVALID      = -1;
    constexpr gl_vertex       GL_VERTEX_INVALID       = -1;
    constexpr gl_buffer       GL_BUFFER_INVALID       = -1;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // shader
    SLD_API_INLINE void gl_shader_create                (gl_shader& shader,       gl_error& error);
    SLD_API_INLINE void gl_shader_destroy               (gl_shader& shader,       gl_error& error, gl_status& status);
    SLD_API_INLINE void gl_shader_link                  (gl_shader& shader,       gl_error& error, gl_status& status);
    SLD_API_INLINE void gl_shader_set_active            (gl_shader& shader,       gl_error& error);
    SLD_API_INLINE void gl_shader_attach_stage          (gl_shader& shader,       gl_error& error, gl_shader_stage& stage);
    SLD_API_INLINE void gl_shader_get_uniform           (gl_shader& shader,       gl_error& error, gl_uniform& uniform, const cchar* name);

    // stage
    SLD_API_INLINE void gl_shader_stage_create_vertex   (gl_shader_stage& stage,  gl_error& error);
    SLD_API_INLINE void gl_shader_stage_create_fragment (gl_shader_stage& stage,  gl_error& error);
    SLD_API_INLINE void gl_shader_stage_destroy         (gl_shader_stage& stage,  gl_error& error);
    SLD_API_INLINE void gl_shader_stage_compile         (gl_shader_stage& stage,  gl_error& error, const cchar* buffer);

    // uniform
    SLD_API_INLINE void gl_uniform_set_s32              (gl_uniform& uniform,     gl_error& error, s32&  value);
    SLD_API_INLINE void gl_uniform_set_u32              (gl_uniform& uniform,     gl_error& error, u32&  value);
    SLD_API_INLINE void gl_uniform_set_f32              (gl_uniform& uniform,     gl_error& error, f32&  value);
    SLD_API_INLINE void gl_uniform_set_vec2             (gl_uniform& uniform,     gl_error& error, vec2& value);
    SLD_API_INLINE void gl_uniform_set_vec3             (gl_uniform& uniform,     gl_error& error, vec3& value);
    SLD_API_INLINE void gl_uniform_set_mat3             (gl_uniform& uniform,     gl_error& error, mat3& value);
    SLD_API_INLINE void gl_uniform_set_mat4             (gl_uniform& uniform,     gl_error& error, mat4& value);

    // vertex
    SLD_API_INLINE void gl_vertex_create                (gl_vertex&    vertex,    gl_error& error);
    SLD_API_INLINE void gl_vertex_destroy               (gl_vertex&    vertex,    gl_error& error);
    SLD_API_INLINE void gl_vertex_enable                (gl_vertex&    vertex,    gl_error& error);
    SLD_API_INLINE void gl_vertex_disable               (gl_vertex&    vertex,    gl_error& error);
    SLD_API_INLINE void gl_vertex_attribute_enable      (gl_attribute& attribute, gl_error& error);
    SLD_API_INLINE void gl_vertex_attribute_disable     (gl_attribute& attribute, gl_error& error);
    SLD_API_INLINE void gl_vertex_attribute_define_s32  (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);
    SLD_API_INLINE void gl_vertex_attribute_define_u32  (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);
    SLD_API_INLINE void gl_vertex_attribute_define_f32  (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);
    SLD_API_INLINE void gl_vertex_attribute_define_vec2 (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);
    SLD_API_INLINE void gl_vertex_attribute_define_vec3 (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);
    SLD_API_INLINE void gl_vertex_attribute_define_mat3 (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);
    SLD_API_INLINE void gl_vertex_attribute_define_mat4 (gl_attribute& attribute, gl_error& error, u32& attribute_offset, u32& vertex_size);

    //-------------------------------------------------------------------
    // SHADER INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_shader_create(
        gl_shader& shader,
        gl_error&  error) {

        shader = glCreateProgram ();
        error  = glGetError      (); 
    }

    SLD_API_INLINE bool
    gl_shader_destroy(
        gl_shader& shader,
        gl_error&  error,
        gl_status& status) {
        
        glDeleteProgram (shader);
        glGetProgramiv  (shader, GL_DELETE_STATUS, &status);

        error = (status == GL_TRUE)
            ? GL_ERROR_SUCCESS
            : glGetError(); 
    }

    SLD_API_INLINE void
    gl_shader_link(
        gl_shader& shader,
        gl_error&  error,
        gl_status& status) {

        glLinkProgram  (shader);
        glGetProgramiv (shader, GL_LINK_STATUS, &status);

        error = (status == GL_TRUE)
            ? GL_ERROR_SUCCESS
            : glGetError();          
    }

    SLD_API_INLINE void
    gl_shader_set_active(
        gl_shader& shader,
        gl_error&  error) {

        glUseProgram(shader);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_shader_attach_stage(
        gl_shader& shader,
        gl_error&  error) {

        glAttachShader(shader, stage);
        error = glGetError();
    }

    SLD_API_INLINE void 
    gl_shader_get_uniform(
        gl_shader&   shader,
        gl_error&    error,
        gl_uniform&  uniform,
        const cchar* name) {
 
        assert(name != NULL);

        uniform = glGetUniformLocation(shader, name);
        error   = glGetError();
    }

    //-------------------------------------------------------------------
    // SHADER STAGE INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_shader_stage_create_vertex(
        gl_shader_stage& stage,
        gl_error&        error) {

        stage = glCreateShader (GL_VERTEX_SHADER);
        error = glGetError     ();
    }

    SLD_API_INLINE void
    gl_shader_stage_create_fragment(
        gl_shader_stage& stage,
        gl_error&        error) {

        stage = glCreateShader(GL_FRAGMENT_SHADER);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_shader_stage_destroy(
        gl_shader_stage& stage,
        gl_error&        error,
        gl_status        status) {

        glDeleteShader (stage);
        glGetShaderiv  (stage, GL_DELETE_STATUS, &status);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_shader_stage_compile(
        gl_shader_stage& stage,
        gl_error&        error,
        gl_status&       status,
        const cchar*     buffer) {

        assert(buffer != NULL);

        glShaderSource  (stage, 1, buffer, NULL);
        glCompileShader (stage);
        glGetShaderiv   (stage, GL_COMPILE_STATUS, &status);

        error = (status == GL_STRUE)
            ? GL_ERROR_SUCCESS
            : glGetError();
    }

    //-------------------------------------------------------------------
    // UNIFORM INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_uniform_set_s32(
        gl_uniform& uniform,
        gl_error&   error,
        s32&        value) {

        glUniform1i(uniform, value);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_uniform_set_u32(
        gl_uniform& uniform,
        gl_error&   error,
        u32&        value) {

        glUniform1ui(uniform, value);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_uniform_set_f32(
        gl_uniform& uniform,
        gl_error&   error,
        f32&        value) {

        glUniform1f(uniform, value);
        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_uniform_set_vec2(
        gl_uniform& uniform,
        gl_error&   error,
        vec2&       value) {
        
        glUniform2f(
            uniform,
            value.x,
            value.y
        );

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_uniform_set_vec3(
        gl_uniform& uniform,
        gl_error&   error,
        vec3&       value) {

        glUniform3f(
            uniform,
            value.x,
            value.y,
            value.z
        );
    
        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_uniform_set_mat3(
        gl_uniform& uniform,
        gl_error&   error,
        mat3&       value) {

        constexpr GLboolean should_transpose = GL_TRUE;
        constexpr GLuint    matrix_count     = 1;
        
        glUniformMatrix3fv(
            uniform,
            matrix_count,
            should_transpose,
            value.buffer
        );
        
        error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_mat4(
        gl_uniform& uniform,
        gl_error&   error,

        const mat4&      value) {

        constexpr GLboolean should_transpose = GL_TRUE;
        constexpr GLuint    matrix_count     = 1;
        
        glUniformMatrix4fv(
            uniform,
            matrix_count,
            should_transpose,
            value.buffer
        );
        
        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }
     
    //-------------------------------------------------------------------
    // VERTEX INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_vertex_create(
        gl_vertex& vertex,
        gl_error&  error) {

        glCreateVertexArrays(1, &vertex);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_vertex_destroy(
        gl_vertex& vertex,
        gl_error&  error) {

        glDeleteVertexArrays(1, &vertex); 
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_vertex_set_active(
        gl_vertex& vertex,
        gl_error&  error) {

        glBindVertexArray(vertex);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_vertex_attribute_enable(
        gl_attribute& attribute,
        gl_error&     error) {

        glEnableVertexAttribArray(attribute);
        error = glGetError();    
    }

    SLD_API_INLINE void
    gl_vertex_attribute_disable(
        gl_attribute& attribute,
        gl_error&     error) {

        glDisableVertexAttribArray(attribute);
        error = glGetError();
    }


    SLD_API_INLINE void
    gl_vertex_attribute_define_s32(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {

        const     void*     attribute_pointer       = (void*)attribute_offset;  
        constexpr GLenum    attribute_type          = GL_INT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 1;

        glVertexAttribPointer(
            attribute,
            attribute_count,
            attribute_type,
            attribute_is_normalized,
            vertex_size,
            attribute_pointer
        );

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_u32(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {

        const void*         attribute_pointer       = (void*)attribute_offset;  
        constexpr GLenum    attribute_type          = GL_UNSIGNED_INT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 1;

        glVertexAttribPointer(
            attribute,
            attribute_count,
            attribute_type,
            attribute_is_normalized,
            vertex_size,
            attribute_pointer
        );

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_f32(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
        
        const     void*     attribute_pointer       = (void*)attribute_offset;  
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 1;

        glVertexAttribPointer(
            attribute,
            attribute_count,
            attribute_type,
            attribute_is_normalized,
            vertex_size,
            attribute_pointer
        );

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_vec2(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
    
        const     void*     attribute_pointer       = (void*)attribute_offset;  
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 2;

        glVertexAttribPointer(
            attribute,
            attribute_count,
            attribute_type,
            attribute_is_normalized,
            vertex_size,
            attribute_pointer
        );

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_vec3(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {

        const     void*     attribute_pointer       = (void*)attribute_offset;  
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 3;

        glVertexAttribPointer(
            attribute,
            attribute_count,
            attribute_type,
            attribute_is_normalized,
            vertex_size,
            attribute_pointer
        );

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_mat3(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
    
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 3;
        constexpr u32       row_size                = sizeof(float) * attribute_count;
        const     void*     row_0_pointer           = (void*)attribute_offset;  
        const     void*     row_1_pointer           = (void*)(attribute_offset + row_size);  
        const     void*     row_2_pointer           = (void*)(attribute_offset + (row_size * 2));  

        glVertexAttribPointer (attribute,     attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_0_pointer);
        glVertexAttribPointer (attribute + 1, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_1_pointer);
        glVertexAttribPointer (attribute + 2, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_2_pointer);

        error = glGetError();
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_mat4(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
    
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 4;
        constexpr u32       row_size                = sizeof(float) * attribute_count;
        const     void*     row_0_pointer           = (void*)attribute_offset;  
        const     void*     row_1_pointer           = (void*)(attribute_offset + row_size);  
        const     void*     row_2_pointer           = (void*)(attribute_offset + (row_size * 2));  
        const     void*     row_3_pointer           = (void*)(attribute_offset + (row_size * 3));  

        glVertexAttribPointer (attribute,     attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_0_pointer);
        glVertexAttribPointer (attribute + 1, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_1_pointer);
        glVertexAttribPointer (attribute + 2, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_2_pointer);
        glVertexAttribPointer (attribute + 3, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_3_pointer);

        error = glGetError();
    }
};

#endif //SLD_GL_SHADER_HPP