#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "shader.h"

namespace proc_gen {
    class Skybox
    {
    public:
        static Skybox* getInstance();
        void draw(const glm::mat4&, const glm::mat4&);

    private:
        static Skybox* skybox_instance;
        Skybox();

        void setupRender();
        void loadCubemap();

        unsigned int cube_map_texture;

        Shader* skybox_shader_ptr;

        unsigned int skybox_VAO, skybox_VBO;

        std::vector<float> skybox_vertices;
        std::vector<std::string> skybox_faces;

        const char* SKYBOX_VERTEX_SHADER = "skybox.vs";
        const char* SKYBOX_FRAGMENT_SHADER = "skybox.fs";
    };
}