//
// Created by inessa on 13/04/16.
//

#include "modelmanager.hpp"
#include <shader.hpp>
#include <controls.hpp>

void ModelManager::ModelManager (char* vertexShaders, char* fragmentShaders, char* lightPosition)
{
    LightID = glGetUniformLocation(programID, lightPosition);
    programID = LoadShaders(vertexShaders, fragmentShaders);
    glGenerateVertextArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    matrixID = glGetUniformLocation(programID, "MVP");
    viewMatrixID = glGetUniformLocation(programID, "V");
}

GLuint ModelManager::getVertexArrayID()
{
    return vertexArrayID;
}

GLuint ModelManager::getProgramID()
{
    return vertexArrayID;
}

GLuint ModelManager::generateMVP(Model &model)
{
    ModelManager::projectionMatrix = getProjectionMatrix();
    ModelManager::viewMatrix = getViewMatrix();
    ModelManager::MVP = projectionMatrix * viewMatrix * model.getModelMatrix();
}

glm::mat4 ModelManager::getProjectionMatrix()
{
    return ModelManager::projectionMatrix;
}

glm::mat4 ModelManager::getViewMatrix()
{
    return ModelManager::viewMatrix;
}

GLuint ModelManager::getMatrixID()
{
    return ModelManager::viewMatrixID;
}

void ModelManager::setProjectionMatrix(glm::mat4 projectionMatrix)
{
    ModelManager::projectionMatrix = projectionMatrix;
}

void ModelManager::setViewMatrix(glm::mat4 viewMatrix)
{
    ModelManager::viewMatrix = viewMatrix;
}

void ModelManager::loadMesh(char* path)
{
    ModelManager::meshes.push_back(Mesh(path, meshes.size()));
}

void ModelManager::InitializeMesh(Mesh &mesh)
{
    mesh.loadMesh();
}

void ModelManager::setLightPosition(glm::vec3 lightPos = glm::vec3(4, 4, 4))
{
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
}

void ModelManager::activateTexture(Model &model)
{
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);  // Model
    glBindTexture(GL_TEXTURE_2D, model.getTexture()); // Model
    // Set our texture sampler to user Texture Unit 0
    glUniform1i(model.getTextureID(), 0); // Model
}


void ModelManager::Draw()
{

}