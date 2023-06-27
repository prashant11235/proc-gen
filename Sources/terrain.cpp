#include "terrain.h"

Terrain::Terrain(int x_size, int z_size)
{
    grid_size_x = x_size;
    grid_size_z = z_size;
}

void Terrain::generate(float x_pos, float z_pos)
{
    vertices.clear();
    normals.clear();
    colours.clear();
    indices.clear();

    generateVertices(x_pos, z_pos);
    generateIndices();
    generateLightingNormals();

}

std::vector<float> Terrain::getVertices()
{
    return vertices;
}

std::vector<unsigned int> Terrain::getIndices()
{
    return indices;
}

std::vector<float> Terrain::getColours()
{
    return colours;
}

std::vector<float> Terrain::getLightingNormals()
{
    return normals;
}

float Terrain::noise(float x, float y)
{
    // Ensures value is between 0 - 1 
    glm::vec2 p(x, y);
    float rval = glm::perlin(p) / 2.0f + 0.5f;
    return rval;
}

void Terrain::generateColourAt(float y_)
{
    if (y_ < DEEP_WATER_LEVEL)
    {
        colours.push_back(0.00f);
        colours.push_back(0.00f);
        colours.push_back(0.502f);
    }

    else if (y_ < SHALLOW_WATER_LEVEL)
    {
        colours.push_back(0.00f);
        colours.push_back(0.00f);
        colours.push_back(1.00f);
    }

    else if (y_ < SHORE_LEVEL)
    {
        colours.push_back(0.000f);
        colours.push_back(0.502f);
        colours.push_back(1.00f);
    }
    else if (y_ < SAND_LEVEL)
    {
        colours.push_back(0.941f);
        colours.push_back(0.941f);
        colours.push_back(0.251f);
    }

    else if (y_ < GRASS_LEVEL)
    {
        colours.push_back(0.125f);
        colours.push_back(0.627f);
        colours.push_back(0.00f);
    }

    else if (y_ < DIRT_LEVEL)
    {

        colours.push_back(0.208f);
        colours.push_back(0.137f);
        colours.push_back(0.114f);
    }

    else if (y_ < ROCK_LEVEL)
    {
        colours.push_back(0.5);
        colours.push_back(0.5);
        colours.push_back(0.5);
    }

    else
    {
        colours.push_back(1.00f);
        colours.push_back(1.00f);
        colours.push_back(1.00f);
    }
}

int Terrain::getGridSizeX()
{
    return grid_size_x;
}

int Terrain::getGridSizeZ()
{
    return grid_size_z;
}

void Terrain::generateVertices(float x_pos, float z_pos)
{
    float max_val = octave_1 + octave_2 + octave_3;
    // Calculate the vertex positions of all the triangles
    for (int z = 0; z <= grid_size_z; z++)
    {
        for (int x = 0; x <= grid_size_x; x++)
        {
            float x_ = (float)(x + x_pos) / (float)grid_size_x;
            float z_ = (float)(z + z_pos) / (float)grid_size_z;

            float y_ = octave_1 * noise(freq_1 * x_, freq_2 * z_);
            y_ += octave_2 * noise(freq_2 * x_, freq_2 * z_);
            y_ += octave_3 * noise(freq_3 * x_, freq_3 * z_);
            generateColourAt(y_ / (octave_1 + octave_2 + octave_3));

            float y = pow(y_, exponent);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }

    }

}

void Terrain::generateIndices()
{
    // Calculate the indices associated with each triangle for drawing
    int vert = 0;
    for (int z = 0; z < grid_size_z; z++)
    {
        for (int x = 0; x < grid_size_x; x++)
        {
            indices.push_back(vert);
            indices.push_back(vert + grid_size_x + 1);
            indices.push_back(vert + 1);
            indices.push_back(vert + 1);
            indices.push_back(vert + grid_size_x + 1);
            indices.push_back(vert + grid_size_x + 2);
            vert++;
        }
        vert++;
    }
}

void Terrain::generateLightingNormals()
{
    for (int i = 0; i < indices.size(); i += 3)
    {
        // Get the indices associated with a triangles
        unsigned int idx_1 = indices[i];
        unsigned int idx_2 = indices[i + 1];
        unsigned int idx_3 = indices[i + 2];

        // Get the vertices associated with the indices 
        // Multiply by 3 because each vertex has 3 points associated to it 
        unsigned int vertex_pos = 3 * idx_1;
        glm::vec3 t1 = glm::vec3(vertices[vertex_pos], vertices[vertex_pos + 1], vertices[vertex_pos + 2]);

        vertex_pos = 3 * idx_2;
        glm::vec3 t2 = glm::vec3(vertices[vertex_pos], vertices[vertex_pos + 1], vertices[vertex_pos + 2]);

        vertex_pos = 3 * idx_3;
        glm::vec3 t3 = glm::vec3(vertices[vertex_pos], vertices[vertex_pos + 1], vertices[vertex_pos + 2]);

        glm::vec3 v1 = t2 - t1;
        glm::vec3 v2 = t3 - t1;

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }
}