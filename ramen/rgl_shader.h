#ifndef RGL_SHADER_H
#define RGL_SHADER_H

#include <glad/glad.h>

#include <stdio.h>
#include <string.h> /* for memset */

#include "rgl_platform.h"

class Shader
{
  public:
    bool Load(const char* vertShaderFile, const char* fragShaderFile)
    {
        GLuint vertShader;
        GLuint fragShader;

        if ( !CompileShader(vertShaderFile, &vertShader, GL_VERTEX_SHADER) )
        {
            fprintf(stderr, "Failed to compile shader: '%s'\n", vertShaderFile);
            return false;
        }
        if ( !CompileShader(fragShaderFile, &fragShader, GL_FRAGMENT_SHADER) )
        {
            fprintf(stderr, "Failed to compile shader: '%s'\n", fragShaderFile);
            return false;
        }

        m_Program = glCreateProgram();
        glAttachShader(m_Program, vertShader);
        glAttachShader(m_Program, fragShader);
        glLinkProgram(m_Program);

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return true;
    }

    void Use()
    {
        glUseProgram(m_Program);
    }

    void Delete()
    {
        glDeleteProgram(m_Program);
    }

  private:
    bool CompileShader(const char* shaderFilename, GLuint* shader, GLenum shaderType)
    {
        if ( !FileExists(shaderFilename) )
        {
            fprintf(stderr, "Shader file: '%s' does not exist.\n", shaderFilename);
            return false;
        }

        /* Read shader source from disk and compile. */
        File shaderFile = ReadFile(shaderFilename);
        *shader         = glCreateShader(shaderType);
        glShaderSource(*shader, 1, &shaderFile.data, nullptr);
        glCompileShader(*shader);
        if ( !IsCompiled(*shader) )
        {
            return false;
        }

        return true;
    }

    bool IsCompiled(GLuint shader)
    {
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if ( status != GL_TRUE )
        {
            char buffer[ 512 ];
            memset(buffer, 0, 512);
            glGetShaderInfoLog(shader, 511, nullptr, buffer);
            printf("GLSL compile error:\n%s\n", buffer);

            return false;
        }

        return true;
    }

  private:
    GLuint m_Program;
};

#endif
