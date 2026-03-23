#ifndef RGL_MODEL_H
#define RGL_MODEL_H

#include <glad/glad.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "rgl_math.h"
#include "rgl_platform.h"

struct Vertex
{
    Vec3f position;
    Vec3f normal;
    Vec3f color;
};

class Model
{
  public:
    bool Load(const char* file)
    {
        if ( !FileExists(file) )
        {
            return false;
        }

        File              modelFile = ReadFile(file);
        tinyobj::attrib_t attrib;
        std::string       err;
        bool              ret = tinyobj::LoadObj(&attrib, &m_Shapes, nullptr, &err, file, "/");
        if ( !ret )
        {
            return false;
        }

        printf("loaded: '%s'\n", file);
        printf("# of vertices: %d\n", (int)(attrib.vertices.size()) / 3);
        printf("# of normals: %d\n", (int)(attrib.normals.size() / 3));
        printf("# of texcoords: %d\n", (int)attrib.texcoords.size() / 2);
        printf("# of shapes: %d\n", (int)m_Shapes.size());

        for ( int i = 0; i < m_Shapes.size(); i++ )
        {
            for ( int j = 0; j < m_Shapes[ i ].mesh.indices.size(); j++ )
            {
                tinyobj::index_t idx = m_Shapes[ i ].mesh.indices[ j ];
                Vertex           v{};
                for ( int k = 0; k < 3; k++ )
                {
                    v.position[ k ] = attrib.vertices[ 3 * idx.vertex_index + k ];
                }
                m_Vertices.push_back(v);
            }
        }
        printf("m_Vertices.size(): %lu\n", m_Vertices.size());

        UploadToGPU();

        modelFile.Destroy();

        return true;
    }

    const GLuint GetBuffer()
    {
        return m_Buffer;
    }

    const size_t NumVertices()
    {
        return m_Vertices.size();
    }

  private:
    void UploadToGPU()
    {
        glCreateBuffers(1, &m_Buffer);
        glNamedBufferData(m_Buffer, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);
    }

    void UnloadFromGPU()
    {
        if ( m_Buffer != 0 )
        {
            glDeleteBuffers(1, &m_Buffer);
        }
    }

  private:
    std::vector<tinyobj::shape_t> m_Shapes;
    std::vector<Vertex>           m_Vertices;
    GLuint                        m_Buffer;
};
#endif
