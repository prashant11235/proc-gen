#include "skybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Skybox* Skybox::skybox_instance = NULL;

Skybox* Skybox::getInstance()
{
    if (!Skybox::skybox_instance)
    {
        Skybox::skybox_instance = new Skybox();
    }
    return Skybox::skybox_instance;
}

Skybox::Skybox()
{
    skybox_vertices =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    skybox_faces =
    {
        "../resources/skybox/daymap/right.jpg",
        "../resources/skybox/daymap/left.jpg",
        "../resources/skybox/daymap/top.jpg",
        "../resources/skybox/daymap/bottom.jpg",
        "../resources/skybox/daymap/front.jpg",
        "../resources/skybox/daymap/back.jpg"
    };

    loadCubemap();
    setupRender();

    skybox_shader_ptr = new Shader(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
    skybox_shader_ptr->use();
    skybox_shader_ptr->setInt("skybox", 0);
}

void Skybox::loadCubemap()
{
    glGenTextures(1, &cube_map_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);

    int width, height, n_Channels;
    for (int i = 0; i < skybox_faces.size(); i++)
    {
        unsigned char* data = stbi_load(skybox_faces[i].c_str(), &width, &height, &n_Channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        else
        {
            std::cout << "Failed to load cube map at " << skybox_faces[i] << "\n";
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::setupRender()
{
    glGenVertexArrays(1, &skybox_VAO);
    glGenBuffers(1, &skybox_VBO);
    glBindVertexArray(skybox_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
    glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(float), &skybox_vertices.front(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Skybox::draw(const glm::mat4& view, const glm::mat4& projection)
{
    glDepthFunc(GL_LEQUAL);
    skybox_shader_ptr->use();

    glm::mat4 tmp_view = glm::mat4(glm::mat3(view));
    skybox_shader_ptr->setMat4("view", tmp_view);
    skybox_shader_ptr->setMat4("projection", projection);

    glBindVertexArray(skybox_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}