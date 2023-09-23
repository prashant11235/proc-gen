#pragma once
#include <glm/gtc/noise.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <math.h>

namespace proc_gen {
    class Terrain
    {
    public:
        Terrain(int, int);
        void generate(float, float);

        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
        std::vector<float> getColours();
        std::vector<float> getLightingNormals();

        int getGridSizeX();
        int getGridSizeZ();

    private:
        // Generation functions 
        void generateColourAt(float y);
        void generateVertices(float, float);
        void generateIndices();
        void generateLightingNormals();

        // Noise function
        float noise(float, float);

        // Parameters that should be used for opengl drawing 
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        std::vector<float> colours;
        std::vector<float> normals;

        // Parameters related to grid size of terrain 
        int grid_size_x;
        int grid_size_z;

        // Parameters related to noise generation 
        float freq_1 = 1.00f;
        float freq_2 = 2.00f;
        float freq_3 = 4.00f;

        float octave_1 = 0.15f;
        float octave_2 = 1.00f;
        float octave_3 = 0.11f;

        float exponent = 7.00f;

        // Parameters related to what height corrosponds to what 
        const float DEEP_WATER_LEVEL = 0.375;
        const float SHALLOW_WATER_LEVEL = 0.5;
        const float SHORE_LEVEL = 0.53125;
        const float SAND_LEVEL = 0.5625;
        const float GRASS_LEVEL = 0.6875;
        const float DIRT_LEVEL = 0.81875;
        const float ROCK_LEVEL = 0.95;
        // Anything over dirt is just snow

    };
}