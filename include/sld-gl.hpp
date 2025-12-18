#ifndef SLD_GL_SHADER_HPP
#define SLD_GL_SHADER_HPP

#include "sld.hpp"
#include "sld-vector.hpp"
#include "sld-matrix.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------
     
    using gl_error          = GLint;
    using gl_uniform        = GLint;
    using gl_status         = GLint; 
    using gl_program = GLuint;
    using gl_shader   = GLuint;
    using gl_vertex         = GLuint; 
    using gl_attribute      = GLuint; 
    using gl_buffer         = GLuint;

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr gl_error   GL_ERROR_SUCCESS   = 0; 
    constexpr gl_program GL_PROGRAM_INVALID = 0xFFFFFFFF;
    constexpr gl_shader  GL_SHADER_INVALID  = 0xFFFFFFFF;
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
    // METHODS
    //-------------------------------------------------------------------

    // context
    SLD_API_INLINE void gl_context_init (void);

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
    // SHADER INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_context_init(
        void) {

        const bool did_init = (glewInit() == GLEW_OK);
        assert(did_init);
    }

    SLD_API_INLINE void
    gl_program_create(
        gl_program& program,
        gl_error&          error) {

        program = glCreateProgram ();
        error   = glGetError      (); 
    }

    SLD_API_INLINE void
    gl_program_destroy(
        gl_program& program,
        gl_error&          error,
        gl_status&         status) {
        
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

        glUseProgram(program);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_program_attach_shader(
        gl_program& program,
        gl_error&   error,
        gl_shader&  shader) {

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

        uniform = glGetUniformLocation(program, name);
        error   = glGetError();
    }

    //-------------------------------------------------------------------
    // SHADER shader INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void
    gl_shader_create_vertex(
        gl_shader& shader,
        gl_error&  error) {

        shader = glCreateShader (GL_VERTEX_SHADER);
        error = glGetError     ();
    }
    
    SLD_API_INLINE void
    gl_shader_create_tessellation_control(
        gl_shader& shader,
        gl_error&  error) {

        shader = glCreateShader (GL_TESS_CONTROL_SHADER);
        error = glGetError     ();
    }

    SLD_API_INLINE void
    gl_shader_create_tessellation_evaluation(
        gl_shader& shader,
        gl_error&  error) {

        shader = glCreateShader (GL_TESS_EVALUATION_SHADER);
        error = glGetError     ();
    }

    SLD_API_INLINE void
    gl_shader_create_geometry(
        gl_shader& shader,
        gl_error&  error) {

        shader = glCreateShader (GL_GEOMETRY_SHADER);
        error = glGetError     ();
    }
    
    SLD_API_INLINE void
    gl_shader_create_fragment(
        gl_shader& shader,
        gl_error&  error) {

        shader = glCreateShader(GL_FRAGMENT_SHADER);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_shader_destroy(
        gl_shader& shader,
        gl_error&  error,
        gl_status  status) {

        glDeleteShader (shader);
        glGetShaderiv  (shader, GL_DELETE_STATUS, &status);
        error = glGetError();
    }

    SLD_API_INLINE void
    gl_shader_compile(
        gl_shader&   shader,
        gl_error&    error,
        gl_status&   status,
        const cchar* buffer) {

        assert(buffer != NULL);

        glShaderSource  (shader, 1, &buffer, NULL);
        glCompileShader (shader);
        glGetShaderiv   (shader, GL_COMPILE_STATUS, &status);

        error = (status == GL_TRUE)
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