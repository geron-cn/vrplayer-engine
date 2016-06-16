#include "GLProgram.h"
#include <string>
#include <assert.h>

namespace vrlive {
    

GLProgram* GLProgram::create(const std::string& vs, const std::string& fs)
{
    GLProgram* program = new GLProgram();
    
    const unsigned int SHADER_SOURCE_LENGTH = 1;
    const GLchar* shaderSource[SHADER_SOURCE_LENGTH];
    char* infoLog = NULL;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLint length;
    GLint success;
    
    std::string tmp = vs;
    shaderSource[0] = tmp.c_str();
    ( vertexShader = glCreateShader(GL_VERTEX_SHADER) );
    ( glShaderSource(vertexShader, 1, shaderSource, NULL) );
    ( glCompileShader(vertexShader) );
    ( glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success) );
    if (success != GL_TRUE)
    {
        ( glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length) );
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            ( glGetShaderInfoLog(vertexShader, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        
        assert(0 && infoLog);
        
        delete[] infoLog;
        
        // Clean up.
        ( glDeleteShader(vertexShader) );
        
        return NULL;
    }

    tmp = fs;
    shaderSource[0] = tmp.c_str();
    ( vertexShader = glCreateShader(GL_FRAGMENT_SHADER) );
    ( glShaderSource(fragmentShader, 1, shaderSource, NULL) );
    ( glCompileShader(fragmentShader) );
    ( glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success) );
    if (success != GL_TRUE)
    {
        ( glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length) );
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            ( glGetShaderInfoLog(fragmentShader, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        
        assert(0 && infoLog);
        
        delete[] infoLog;
        
        // Clean up.
        ( glDeleteShader(fragmentShader) );
        glDeleteShader(vertexShader);
        return NULL;
    }
    
    //link
    GLuint p;
    ( p = glCreateProgram() );
    ( glAttachShader(p, vertexShader) );
    ( glAttachShader(p, fragmentShader) );
    ( glLinkProgram(p) );
    ( glGetProgramiv(p, GL_LINK_STATUS, &success) );
    
    // Delete shaders after linking.
    ( glDeleteShader(vertexShader) );
    ( glDeleteShader(fragmentShader) );
    // Check link status.
    if (success != GL_TRUE)
    {
        ( glGetProgramiv(p, GL_INFO_LOG_LENGTH, &length) );
        if (length == 0)
        {
            length = 4096;
        }
        if (length > 0)
        {
            infoLog = new char[length];
            ( glGetProgramInfoLog(p, length, NULL, infoLog) );
            infoLog[length-1] = '\0';
        }
        assert(0 && "Linking program failed");
        delete[] (infoLog);
        
        // Clean up.
        ( glDeleteProgram(p) );
        
        return NULL;
    }
    
    GLint activeAttributes;
    ( glGetProgramiv(p, GL_ACTIVE_ATTRIBUTES, &activeAttributes) );
    if (activeAttributes > 0)
    {
        ( glGetProgramiv(p, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length) );
        if (length > 0)
        {
            GLchar* attribName = new GLchar[length + 1];
            GLint attribSize;
            GLenum attribType;
            GLint attribLocation;
            for (int i = 0; i < activeAttributes; ++i)
            {
                // Query attribute info.
                ( glGetActiveAttrib(p, i, length, NULL, &attribSize, &attribType, attribName) );
                attribName[length] = '\0';
                
                // Query the pre-assigned attribute location.
                ( attribLocation = glGetAttribLocation(p, attribName) );
                
                // Assign the vertex attribute mapping for the effect.
                program->_attributes[attribName] = attribLocation;
            }
            delete[] (attribName);
        }
    }

    GLint activeUniforms;
    ( glGetProgramiv(p, GL_ACTIVE_UNIFORMS, &activeUniforms) );
    if (activeUniforms > 0)
    {
        ( glGetProgramiv(p, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length) );
        if (length > 0)
        {
            GLchar* uniformName = new GLchar[length + 1];
            GLint uniformSize;
            GLenum uniformType;
            GLint uniformLocation;
            unsigned int samplerIndex = 0;
            for (int i = 0; i < activeUniforms; ++i)
            {
                // Query uniform info.
                ( glGetActiveUniform(p, i, length, NULL, &uniformSize, &uniformType, uniformName) );
                uniformName[length] = '\0';  // null terminate
                if (length > 3)
                {
                    // If this is an array uniform, strip array indexers off it since GL does not
                    // seem to be consistent across different drivers/implementations in how it returns
                    // array uniforms. On some systems it will return "u_matrixArray", while on others
                    // it will return "u_matrixArray[0]".
                    char* c = strrchr(uniformName, '[');
                    if (c)
                    {
                        *c = '\0';
                    }
                }
                
                // Query the pre-assigned uniform location.
                ( uniformLocation = glGetUniformLocation(p, uniformName) );
                
                program->_uniforms[uniformName] = uniformLocation;
            }
            delete[] (uniformName);
        }
    }
    
    program->_program = p;
    
    return program;
}
    
    void GLProgram::bind()
    {
        if (_program)
        {
            glUseProgram(_program);
        }
    }
    
    void GLProgram::unbind()
    {
        if (_program)
        {
            glUseProgram(0);
        }
    }
    
    GLuint GLProgram::getAttribute(const std::string& attributName)
    {
        auto it = _attributes.find(attributName);
        if (it != _attributes.end())
        {
            return it->second;
        }
        assert(0);
        return -1;
    }
    
    GLuint GLProgram::getUniform(const std::string& uniformName)
    {
        auto it = _uniforms.find(uniformName);
        if (it != _uniforms.end())
        {
            return it->second;
        }
        assert(0);
        return -1;
    }

GLProgram::GLProgram()
    : _program(0)
{
    
}

/**
 * Destructor.
 */
GLProgram::~GLProgram()
{
    
}

}
