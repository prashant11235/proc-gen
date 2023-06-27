#include "terrain_manager.h"

TerrainManager* TerrainManager::terrain_manager_instance = NULL;

TerrainManager* TerrainManager::getInstance()
{
    if (!TerrainManager::terrain_manager_instance)
    {
        TerrainManager::terrain_manager_instance = new TerrainManager();
    }
    return TerrainManager::terrain_manager_instance;
}

TerrainManager::TerrainManager()
{
    terrain_shader_ptr = new Shader(TERRAIN_VERTEX_SHADER, TERRAIN_FRAGMENT_SHADER);

    texture_grass = new Texture("../resources/textures/grass.jpg", GL_TEXTURE_2D);
    UniformID1 = glGetUniformLocation(terrain_shader_ptr->ID, "grassTex");
    std::cout << "Uniform " << UniformID1 << std::endl;

    texture_stone = new Texture("../resources/textures/stone.jpg", GL_TEXTURE_2D);
    UniformID2 = glGetUniformLocation(terrain_shader_ptr->ID, "stoneTex");
    std::cout << "Uniform " << UniformID2 << std::endl;

    texture_water = new Texture("../resources/textures/water.jpg", GL_TEXTURE_2D);
    UniformID3 = glGetUniformLocation(terrain_shader_ptr->ID, "waterTex");
    std::cout << "Uniform " << UniformID3 << std::endl;
    
    for (int i = 0; i < NUM_TERRAINS; i++)
    {
        terrains[i] = new Terrain(GRID_SIZE_X, GRID_SIZE_Z);
        setupRender(terrains[i], terrain_VAOs[i], terrain_VBOs[i], EBOs[i]);
    }
}

int TerrainManager::getGridSizeX()
{
    return GRID_SIZE_X;
}

int TerrainManager::getGridSizeZ()
{
    return GRID_SIZE_Z;
}

void TerrainManager::draw(const glm::vec3& camera_pos, const glm::mat4& view, const glm::mat4& projection)
{
    terrain_shader_ptr->use();
    terrain_shader_ptr->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
    terrain_shader_ptr->setVec3("viewPos", camera_pos);

    // light properties 
    terrain_shader_ptr->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    terrain_shader_ptr->setVec3("light.diffuse", 0.6f, 0.6f, 0.6f);
    terrain_shader_ptr->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    terrain_shader_ptr->setMat4("projection", projection);
    terrain_shader_ptr->setMat4("view", view);

    glUniform1i(UniformID1, 0);
    glUniform1i(UniformID2, 1);
    glUniform1i(UniformID3, 2);

    // TODO: clean this part 
    for (int i = 0; i < NUM_TERRAINS_SQRT; i++)
    {
        float z_trans = floor(camera_pos.z / GRID_SIZE_Z) * GRID_SIZE_Z;

        if (i == 0)
            z_trans += GRID_SIZE_Z;
        if (i == 1)
            z_trans += 0.00f;
        if (i == 2)
            z_trans -= GRID_SIZE_Z;

        for (int j = 0; j < NUM_TERRAINS_SQRT; j++)
        {
            float x_trans = floor(camera_pos.x / GRID_SIZE_X) * GRID_SIZE_X;

            if (j == 0)
                x_trans -= GRID_SIZE_X;
            if (j == 1)
                x_trans += 0.00f;
            if (j == 2)
                x_trans += GRID_SIZE_X;

            int terrain_idx = NUM_TERRAINS_SQRT * i + j;
            updateBuffer(terrains[terrain_idx], x_trans, z_trans, terrain_VBOs[terrain_idx], EBOs[terrain_idx]);
            glBindVertexArray(terrain_VAOs[terrain_idx]);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(x_trans, 0, z_trans));

            terrain_shader_ptr->setMat4("model", model);

            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, texture_grass->ID);

            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, texture_stone->ID);

            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, texture_water->ID);

            glDrawElements(GL_TRIANGLES, terrains[terrain_idx]->getIndices().size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        }
    }

}

void TerrainManager::updateBuffer(Terrain* terrain, float x, float z, unsigned int terrain_VBOs[NUM_VBO_MODES], unsigned int& EBO)
{
    // Generate terrain attributes 
    terrain->generate(x, z);
    terrain_vertices = terrain->getVertices();
    terrain_indices = terrain->getIndices();
    terrain_colours = terrain->getColours();
    terrain_normals = terrain->getLightingNormals();

    // Bind vertices to the first terrain_VBOs 
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, terrain_vertices.size() * sizeof(float), &terrain_vertices.front(), GL_STATIC_DRAW);

    // Bind indices to EBO for drawing 
    // Not too sure why but this must be done before doing things with colours 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrain_indices.size() * sizeof(float), &terrain_indices.front(), GL_STATIC_DRAW);

    // Do the same as above but for colours 
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, terrain_colours.size() * sizeof(float), &terrain_colours.front(), GL_STATIC_DRAW);

    // Do the same as above but for normals 
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, terrain_normals.size() * sizeof(float), &terrain_normals.front(), GL_STATIC_DRAW);

    // Do the same as above but for textures
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[3]);
    glBufferData(GL_ARRAY_BUFFER, terrain_vertices.size() * sizeof(float), &terrain_vertices.front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TerrainManager::setupRender(Terrain* terrain, unsigned int& terrain_VAOs, unsigned int* terrain_VBOs, unsigned int& EBO)
{
    // Generate terrain attributes 
    terrain->generate(0.0f, 0.0f);
    terrain_vertices = terrain->getVertices();
    terrain_indices = terrain->getIndices();
    terrain_colours = terrain->getColours();
    terrain_normals = terrain->getLightingNormals();

    // Generate all buffers and terrain_VAOs necessary
    glGenVertexArrays(1, &terrain_VAOs);
    glGenBuffers(3, terrain_VBOs);
    glGenBuffers(1, &EBO);

    // Bind terrain_VAOs 
    glBindVertexArray(terrain_VAOs);

    // Bind vertices to the first terrain_VBOs 
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, terrain_vertices.size() * sizeof(float), &terrain_vertices.front(), GL_STATIC_DRAW);

    // Bind indices to EBO for drawing 
    // Not too sure why but this must be done before doing things with colours 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrain_indices.size() * sizeof(float), &terrain_indices.front(), GL_STATIC_DRAW);

    // Configure vertex attributes for vertices 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Do the same as above but for colours 
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, terrain_colours.size() * sizeof(float), &terrain_colours.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Do the same as above but for normals 
    glBindBuffer(GL_ARRAY_BUFFER, terrain_VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, terrain_normals.size() * sizeof(float), &terrain_normals.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}