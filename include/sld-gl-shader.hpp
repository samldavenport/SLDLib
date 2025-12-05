#ifndef SLD_GL_SHADER_HPP
#define SLD_GL_SHADER_HPP

#include "sld.hpp"
#include "sld-vector.hpp"
#include "sld-matrix.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u32 GL_VERTEX_ATTRIBUTE_SINGLE_VALUE = 1;
    constexpr u32 GL_VERTEX_ATTRIBUTE_NO_PADDING   = 0;

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------
    
    using  gl_shader       = GLuint;
    using  gl_shader_stage = GLuint;
    using  gl_uniform      = GLint;
    using  gl_vertex       = GLuint; 
    using  gl_buffer       = GLuint;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // shader
    SLD_API_INLINE const gl_shader       gl_shader_create                    (void);
    SLD_API_INLINE bool                  gl_shader_destroy                   (const gl_shader shader);
    SLD_API_INLINE bool                  gl_shader_link                      (const gl_shader shader);
    SLD_API_INLINE bool                  gl_shader_unlink                    (const gl_shader shader);
    SLD_API_INLINE bool                  gl_shader_set_active                (const gl_shader shader);
    SLD_API_INLINE bool                  gl_shader_attach_stage              (const gl_shader shader, const gl_shader_stage stage);
    SLD_API_INLINE const gl_uniform      gl_shader_get_uniform               (const gl_shader shader, const cchar* name);


    // shader
    SLD_API_INLINE const gl_shader_stage gl_shader_stage_create_vertex       (void);
    SLD_API_INLINE const gl_shader_stage gl_shader_stage_create_fragment     (void);
    SLD_API_INLINE bool                  gl_shader_stage_destroy             (const gl_shader_stage stage);
    SLD_API_INLINE bool                  gl_shader_stage_compile             (const gl_shader_stage stage, const cchar* buffer);

    // uniform
    SLD_API_INLINE bool                  gl_uniform_set_s32                  (const gl_uniform uniform, const s32   value);
    SLD_API_INLINE bool                  gl_uniform_set_u32                  (const gl_uniform uniform, const u32   value);
    SLD_API_INLINE bool                  gl_uniform_set_f32                  (const gl_uniform uniform, const f32   value);
    SLD_API_INLINE bool                  gl_uniform_set_vec2                 (const gl_uniform uniform, const vec2& value);
    SLD_API_INLINE bool                  gl_uniform_set_vec3                 (const gl_uniform uniform, const vec3& value);
    SLD_API_INLINE bool                  gl_uniform_set_mat3                 (const gl_uniform uniform, const mat3& value);
    SLD_API_INLINE bool                  gl_uniform_set_mat4                 (const gl_uniform uniform, const mat4& value);

    // vertex
    SLD_API_INLINE const gl_vertex       gl_vertex_create                    (void);
    SLD_API_INLINE void                  gl_vertex_destroy                   (const gl_vertex vertex);
    SLD_API_INLINE void                  gl_vertex_enable                    (const gl_vertex vertex);
    SLD_API_INLINE void                  gl_vertex_disable                   (const gl_vertex vertex);
    SLD_API_INLINE void                  gl_vertex_attribute_define_s32      (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_define_u32      (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_define_f32      (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_define_vec2     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_define_vec3     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_define_mat3     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_define_mat4     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE void                  gl_vertex_attribute_enable          (const u32 attribute_index);
    SLD_API_INLINE void                  gl_vertex_attribute_disable         (const u32 attribute_index);

    //-------------------------------------------------------------------
    // SHADER INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE const gl_shader
    gl_shader_create(
        void) {

        const gl_shader shader = glCreateProgram();
        return(shader);
    }

    SLD_API_INLINE bool
    gl_shader_destroy(
        const gl_shader shader) {
        
        glDeleteProgram(shader);

        GLint delete_result;
        glGetProgramiv(shader, GL_DELETE_STATUS, &delete_result);

        return(delete_result == GL_TRUE);
    }

    SLD_API_INLINE bool
    gl_shader_link(
        const gl_shader shader) {

        glLinkProgram(shader);

        GLint link_result;
        glGetProgramiv(shader, GL_LINK_STATUS, &link_result);

        return(link_result == GL_TRUE);         
    }

    SLD_API_INLINE bool
    gl_shader_unlink(
        const gl_shader shader) {

        //TODO
    }

    SLD_API_INLINE bool
    gl_shader_set_active(
        const gl_shader shader) {

        glUseProgram(shader);
    }

    SLD_API_INLINE bool
    gl_shader_attach_stage(
        const gl_shader       shader,
        const gl_shader_stage stage) {

        glAttachShader(shader, stage);
    }

    SLD_API_INLINE const gl_uniform
    gl_shader_get_uniform(
        const gl_shader shader,
        const cchar*    name) {

        assert(name != NULL);

        const gl_uniform uniform = glGetUniformLocation(shader, name);
        return(uniform);
    }

    //-------------------------------------------------------------------
    // SHADER STAGE INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE const gl_shader_stage
    gl_shader_stage_create_vertex(
        void) {

        const gl_shader_stage stage = glCreateShader(GL_VERTEX_SHADER);
        return(stage);
    }

    SLD_API_INLINE const gl_shader_stage
    gl_shader_stage_create_fragment(
        void) {

        const gl_shader_stage stage = glCreateShader(GL_FRAGMENT_SHADER);
        return(stage);
    }

    SLD_API_INLINE bool
    gl_shader_stage_destroy(
        const gl_shader_stage stage) {

        glDeleteShader(stage);

        GLint delete_result;
        glGetShaderiv(stage, GL_DELETE_STATUS, &delete_result);

        return(delete_result == GL_TRUE);
    }

    SLD_API_INLINE bool
    gl_shader_stage_compile(
        const gl_shader_stage stage,
        const cchar*          buffer) {

        assert(buffer != NULL);

        glShaderSource  (shader, 1, buffer, NULL);
        glCompileShader (shader);

        GLint delete_result;
        glGetShaderiv(stage, GL_COMPILE_STATUS, &delete_result);

        return(delete_result == GL_TRUE);
    }

    //-------------------------------------------------------------------
    // UNIFORM INLINE METHODS
    //-------------------------------------------------------------------


    SLD_API_INLINE bool
    gl_uniform_set_s32(
        const gl_uniform uniform,
        const s32        value) {

        glUniform1i(uniform, value);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_u32(
        const gl_uniform uniform,
        const u32        value) {

        glUniform1ui(uniform, value);
        
        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_f32(
        const gl_uniform uniform,
        const f32        value) {

        glUniform1f(uniform, value);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_vec2(
        const gl_uniform uniform,
        const vec2&      value) {

        glUniform2f(uniform, value.x, value.y);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_vec3(
        const gl_uniform uniform,
        const vec3&      value) {

        glUniform3f(uniform, value.x, value.y, value.z);
    
        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_mat3(
        const gl_uniform uniform,
        const mat3&      value) {

        constexpr GLboolean should_transpose = GL_TRUE;
        constexpr GLuint    matrix_count     = 1;
        glUniformMatrix3fv(uniform, matrix_count, should_transpose, value.buffer);
        
        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_uniform_set_mat4(
        const gl_uniform uniform,
        const mat4&      value) {

        constexpr GLboolean should_transpose = GL_TRUE;
        constexpr GLuint    matrix_count     = 1;
        glUniformMatrix4fv(uniform, matrix_count, should_transpose, value.buffer);
        
        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }
     
    //-------------------------------------------------------------------
    // VERTEX INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE const gl_vertex
    gl_vertex_create(
        void) {

        gl_vertex vertex = 0;
        glCreateVertexArrays(1, &vertex);

        return(vertex);
    }

    SLD_API_INLINE bool
    gl_vertex_destroy(
        const gl_vertex vertex) {

        glDeleteVertexArrays(1, &vertex); 

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);        
    }

    SLD_API_INLINE bool
    gl_vertex_enable(
        const gl_vertex vertex) {

        glBindVertexArray(vertex);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE void
    gl_vertex_disable(
        const gl_vertex vertex) {

        glBindVertexArray(0);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);        
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_define_s32(
        const u32 attribute_index,
        const u32 attribute_offset,
        const u32 attribute_count,
        const u32 vertex_size) {

        const void*     attribute_pointer       = (void*)attribute_offset;  
        const GLenum    attribute_type          = GL_INT;
        const GLBoolean attribute_is_normalized = GL_FALSE;

        glVertexAttribPointer(
            attribute_index,
            attribute_count,
            attribute_type,
            attribute_is_normalized,
            vertex_size,
            attribute_pointer
        );

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);  
    }

    SLD_API_INLINE bool                  gl_vertex_attribute_define_u32      (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE bool                  gl_vertex_attribute_define_f32      (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE bool                  gl_vertex_attribute_define_vec2     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE bool                  gl_vertex_attribute_define_vec3     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE bool                  gl_vertex_attribute_define_mat3     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    SLD_API_INLINE bool                  gl_vertex_attribute_define_mat4     (const u32 attribute_index, const u32 attribute_offset, const u32 attribute_count, const u32 vertex_size);
    
    SLD_API_INLINE bool
    gl_vertex_attribute_enable(
        const u32 attribute_index) {

        glEnableVertexAttribArray(attribute_index);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }

    SLD_API_INLINE bool
    gl_vertex_attribute_disable(
        const u32 attribute_index) {

        glDisableVertexAttribArray(attribute_index);

        const GLenum error = glGetError();
        return(error == GL_NO_ERROR);
    }
};

#endif //SLD_GL_SHADER_HPP