#ifndef SLD_GL_SHADER_HPP
#define SLD_GL_SHADER_HPP

#include "sld.hpp"
#include "sld-vector.hpp"
#include "sld-matrix.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------
     
    using gl_error         = GLint;
    using gl_uniform       = GLint;
    using gl_status        = GLint; 
    using gl_program       = GLuint;
    using gl_shader        = GLuint;
    using gl_vertex        = GLuint; 
    using gl_attribute     = GLuint; 
    using gl_buffer        = GLuint;
    using gl_buffer_type   = GLenum;
    using gl_buffer_useage = GLenum;

    //-------------------------------------------------------------------
    // METHODS
    //-------------------------------------------------------------------

    // context
    SLD_API_INLINE void gl_context_init                   (void);
    SLD_API_INLINE void gl_context_clear_errors           (void);
    SLD_API_INLINE void gl_context_enable_depth_buffering (gl_error& error);
    SLD_API_INLINE void gl_context_enable_smoothing       (gl_error& error);

    // program
    SLD_API_INLINE void gl_program_create        (gl_program& program, gl_error& error);
    SLD_API_INLINE void gl_program_destroy       (gl_program& program, gl_error& error, gl_status& status);
    SLD_API_INLINE void gl_program_link          (gl_program& program, gl_error& error, gl_status& status);
    SLD_API_INLINE void gl_program_set_active    (gl_program& program, gl_error& error);
    SLD_API_INLINE void gl_program_attach_shader (gl_program& program, gl_error& error, gl_shader& shader);
    SLD_API_INLINE void gl_program_get_uniform   (gl_program& program, gl_error& error, gl_uniform& uniform, const cchar* name);

    // shader
    SLD_API_INLINE void gl_shader_create_vertex                  (gl_shader& shader,  gl_error& error);
    SLD_API_INLINE void gl_shader_create_tessellation_control    (gl_shader& shader,  gl_error& error);
    SLD_API_INLINE void gl_shader_create_tessellation_evaluation (gl_shader& shader,  gl_error& error);
    SLD_API_INLINE void gl_shader_create_geometry                (gl_shader& shader,  gl_error& error);
    SLD_API_INLINE void gl_shader_create_fragment                (gl_shader& shader,  gl_error& error);
    SLD_API_INLINE void gl_shader_destroy                        (gl_shader& shader,  gl_error& error);
    SLD_API_INLINE void gl_shader_compile                        (gl_shader& shader,  gl_error& error, gl_status& status, const cchar* buffer);

    // buffer
    SLD_API_INLINE void gl_buffer_create    (gl_buffer* buffer, gl_error& error, const u32 count);
    SLD_API_INLINE void gl_buffer_destroy   (gl_buffer* buffer, gl_error& error, const u32 count);
    SLD_API_INLINE void gl_buffer_bind_type (gl_buffer& buffer, gl_error& error, const gl_buffer_type type);
    SLD_API_INLINE void gl_buffer_set_data  (gl_buffer& buffer, gl_error& error, const gl_buffer_type type);

    // uniform
    SLD_API_INLINE void gl_uniform_set_s32  (gl_uniform& uniform, gl_error& error, s32&  value);
    SLD_API_INLINE void gl_uniform_set_u32  (gl_uniform& uniform, gl_error& error, u32&  value);
    SLD_API_INLINE void gl_uniform_set_f32  (gl_uniform& uniform, gl_error& error, f32&  value);
    SLD_API_INLINE void gl_uniform_set_vec2 (gl_uniform& uniform, gl_error& error, vec2& value);
    SLD_API_INLINE void gl_uniform_set_vec3 (gl_uniform& uniform, gl_error& error, vec3& value);
    SLD_API_INLINE void gl_uniform_set_mat3 (gl_uniform& uniform, gl_error& error, mat3& value);
    SLD_API_INLINE void gl_uniform_set_mat4 (gl_uniform& uniform, gl_error& error, mat4& value);

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
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr gl_error   GL_ERROR_SUCCESS   = 0; 
    constexpr gl_program GL_PROGRAM_INVALID = 0xFFFFFFFF;
    constexpr gl_shader  GL_SHADER_INVALID  = 0;
    constexpr gl_uniform GL_UNIFORM_INVALID = -1;
    constexpr gl_vertex  GL_VERTEX_INVALID  = -1;
    constexpr gl_buffer  GL_BUFFER_INVALID  = -1;

    constexpr cchar GL_HELLO_TRIANGLE_SHADER_VERTEX[] = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    constexpr cchar GL_HELLO_TRIANGLE_SHADER_FRAGMENT[] =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum gl_buffer_type_ {
        gl_buffer_type_atomic_counter     = 0,
        gl_buffer_type_copy_read          = 1,
        gl_buffer_type_copy_write         = 2,
        gl_buffer_type_dispatch_indirect  = 3,
        gl_buffer_type_draw_indirect      = 4,
        gl_buffer_type_element_array      = 5,
        gl_buffer_type_pixel_pack         = 6,
        gl_buffer_type_pixel_unpack       = 7,
        gl_buffer_type_query              = 9,
        gl_buffer_type_shader_storage     = 10,
        gl_buffer_type_texture            = 11,
        gl_buffer_type_transform_feedback = 12,
        gl_buffer_type_uniform            = 13,
        gl_buffer_type_count              = 14
    };

    enum gl_buffer_useage_ {
        gl_buffer_useage_stream_read  = 0,
        gl_buffer_useage_stream_copy  = 1,
        gl_buffer_useage_static_draw  = 2,
        gl_buffer_useage_static_read  = 3,
        gl_buffer_useage_static_copy  = 4,
        gl_buffer_useage_dynamic_draw = 5,
        gl_buffer_useage_dynamic_read = 6,
        gl_buffer_useage_dynamic_copy = 7,
        gl_buffer_useage_count        = 8
    };

    //-------------------------------------------------------------------
    // CONTEXT
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_context_init(
        void) {

  
    }
    

    SLD_API_INLINE void
    gl_context_enable_depth_buffering(
        gl_error& error) {


    }

    //-------------------------------------------------------------------
    // PROGRAM
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_program_create(
        gl_program& program,
        gl_error&   error) {

        gl_context_clear_errors();

        program = glCreateProgram ();
        error   = (program == GL_PROGRAM_INVALID) ? glGetError() : GL_ERROR_SUCCESS; 
    }

    SLD_API_INLINE void
    gl_program_destroy(
        gl_program& program,
        gl_error&          error,
        gl_status&         status) {

        gl_context_clear_errors();

        glDeleteProgram (program);
        glGetProgramiv  (program, GL_DELETE_STATUS, &status);

        error = (status == GL_TRUE)
            ? GL_ERROR_SUCCESS
            : glGetError(); 
    }

    SLD_API_INLINE void
    gl_program_link(
        gl_program& program,
        gl_error&   error,
        gl_status&  status) {

        gl_context_clear_errors();
        
        glLinkProgram  (program);
        glGetProgramiv (program, GL_LINK_STATUS, &status);

        error = (status == GL_TRUE)
            ? GL_ERROR_SUCCESS
            : glGetError();          
    }

    SLD_API_INLINE void
    gl_program_set_active(
        gl_program& program,
        gl_error&   error) {

        gl_context_clear_errors();
        
        glUseProgram(program);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_program_attach_shader(
        gl_program& program,
        gl_error&   error,
        gl_shader&  shader) {

        gl_context_clear_errors();
        
        glAttachShader(program, shader);
        error = glGetError();
    }

    SLD_API_INLINE void 
    gl_program_get_uniform(
        gl_program&  program,
        gl_error&    error,
        gl_uniform&  uniform,
        const cchar* name) {
 
        assert(name != NULL);

        gl_context_clear_errors();
        
        uniform = glGetUniformLocation(program, name);
        error   = glGetError();
    }

    //-------------------------------------------------------------------
    // SHADER
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_shader_create_vertex(
        gl_shader& shader,
        gl_error&  error) {

        gl_context_clear_errors();
        
        shader = glCreateShader (GL_VERTEX_SHADER);
        error  = (shader == GL_SHADER_INVALID) ? glGetError() : GL_ERROR_SUCCESS;
    }
    
    SLD_API_INLINE void
    gl_shader_create_tessellation_control(
        gl_shader& shader,
        gl_error&  error) {

        gl_context_clear_errors();
        
        shader = glCreateShader (GL_TESS_CONTROL_SHADER);
        error  = (shader == GL_SHADER_INVALID) ? glGetError() : GL_ERROR_SUCCESS;
    }

    SLD_API_INLINE void
    gl_shader_create_tessellation_evaluation(
        gl_shader& shader,
        gl_error&  error) {

        gl_context_clear_errors();
        
        shader = glCreateShader (GL_TESS_EVALUATION_SHADER);
        error  = (shader == GL_SHADER_INVALID) ? glGetError() : GL_ERROR_SUCCESS;
    }

    SLD_API_INLINE void
    gl_shader_create_geometry(
        gl_shader& shader,
        gl_error&  error) {

        gl_context_clear_errors();
        
        shader = glCreateShader (GL_GEOMETRY_SHADER);
        error  = (shader == GL_SHADER_INVALID) ? glGetError() : GL_ERROR_SUCCESS;
    }
    
    SLD_API_INLINE void
    gl_shader_create_fragment(
        gl_shader& shader,
        gl_error&  error) {

        gl_context_clear_errors();
        
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        error  = (shader == GL_SHADER_INVALID) ? glGetError() : GL_ERROR_SUCCESS;
    }

    SLD_API_INLINE void
    gl_shader_destroy(
        gl_shader& shader,
        gl_error&  error,
        gl_status  status) {

        gl_context_clear_errors();
        
        glDeleteShader (shader);
        glGetShaderiv  (shader, GL_DELETE_STATUS, &status);

        error = (status == GL_TRUE) ? GL_ERROR_SUCCESS : glGetError();
    }

    SLD_API_INLINE void
    gl_shader_compile(
        gl_shader&   shader,
        gl_error&    error,
        gl_status&   status,
        const cchar* buffer) {

        gl_context_clear_errors();
        
        assert(buffer != NULL);

        glShaderSource  (shader, 1, &buffer, NULL);
        glCompileShader (shader);
        glGetShaderiv   (shader, GL_COMPILE_STATUS, &status);

        error = (status == GL_TRUE) ? GL_ERROR_SUCCESS : glGetError();
    }

    //-------------------------------------------------------------------
    // BUFFER
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_buffer_create(
        gl_buffer* buffer,
        gl_error&  error, 
        const s32  count) {

        assert(buffer);
        gl_context_clear_errors();
        glGenBuffers(count, buffer);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_buffer_destroy(
        gl_buffer* buffer,
        gl_error&  error, 
        const s32  count) {

        assert(buffer);
        gl_context_clear_errors();
        glDeleteBuffers(count, buffer);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_buffer_bind_type_vertex(
        gl_buffer& buffer,
        gl_error&  error) {
        
        gl_context_clear_errors();
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_buffer_bind_type_index(
        gl_buffer& buffer,
        gl_error&  error) {

        gl_context_clear_errors();
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        error = glGetError();
    }



    //-------------------------------------------------------------------
    // UNIFORM
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

    SLD_API_INLINE void
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

    SLD_API_INLINE void
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

    SLD_API_INLINE void
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
    }

    SLD_API_INLINE void
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
        
        error = glGetError(); 
    }
     
    //-------------------------------------------------------------------
    // VERTEX
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

        

        const     void*     attribute_pointer       = (void*)((addr)attribute_offset);  
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

    SLD_API_INLINE void
    gl_vertex_attribute_define_u32(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {

        const void*         attribute_pointer       = (void*)(addr)attribute_offset;  
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

    SLD_API_INLINE void
    gl_vertex_attribute_define_f32(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
        
        const     void*     attribute_pointer       = (void*)(addr)attribute_offset;  
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

    SLD_API_INLINE void
    gl_vertex_attribute_define_vec2(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
    
        const     void*     attribute_pointer       = (void*)(addr)attribute_offset;  
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

    SLD_API_INLINE void
    gl_vertex_attribute_define_vec3(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {

        const     void*     attribute_pointer       = (void*)(addr)attribute_offset;  
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

    SLD_API_INLINE void
    gl_vertex_attribute_define_mat3(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
    
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 3;
        constexpr u32       row_size                = sizeof(float) * attribute_count;
        const     void*     row_0_pointer           = (void*)(addr)attribute_offset;  
        const     void*     row_1_pointer           = (void*)(addr)(attribute_offset + row_size);  
        const     void*     row_2_pointer           = (void*)(addr)(attribute_offset + (row_size * 2));  

        glVertexAttribPointer (attribute,     attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_0_pointer);
        glVertexAttribPointer (attribute + 1, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_1_pointer);
        glVertexAttribPointer (attribute + 2, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_2_pointer);

        error = glGetError();
    }

    SLD_API_INLINE void
    gl_vertex_attribute_define_mat4(
        gl_attribute& attribute,
        gl_error&     error,
        u32&          attribute_offset,
        u32&          vertex_size) {
    
        constexpr GLenum    attribute_type          = GL_FLOAT;
        constexpr GLboolean attribute_is_normalized = GL_FALSE;
        constexpr u32       attribute_count         = 4;
        constexpr u32       row_size                = sizeof(float) * attribute_count;
        const     void*     row_0_pointer           = (void*)(addr)(attribute_offset);  
        const     void*     row_1_pointer           = (void*)(addr)(attribute_offset + row_size);  
        const     void*     row_2_pointer           = (void*)(addr)(attribute_offset + (row_size * 2));  
        const     void*     row_3_pointer           = (void*)(addr)(attribute_offset + (row_size * 3));  

        glVertexAttribPointer (attribute,     attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_0_pointer);
        glVertexAttribPointer (attribute + 1, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_1_pointer);
        glVertexAttribPointer (attribute + 2, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_2_pointer);
        glVertexAttribPointer (attribute + 3, attribute_count, attribute_type, attribute_is_normalized, vertex_size, row_3_pointer);

        error = glGetError();
    }
};

#endif //SLD_GL_SHADER_HPP