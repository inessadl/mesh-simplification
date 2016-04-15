//
// Created by inessa on 13/04/16.
//

#include "modelmanager.hpp"
#include <shader.hpp>
#include <controls.hpp>

void ModelManager::ModelManager (char* vertexShaders, char* fragmentShaders, char* lightPosition)
{

    glGenerateVertextArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    programID = LoadShaders(vertexShaders, fragmentShaders);
    lightID = glGetUniformLocation(programID, lightPosition);
    matrixID = glGetUniformLocation(programID, "MVP");
    viewMatrixID = glGetUniformLocation(programID, "V");
}

GLuint ModelManager::getVertexArrayID()
{
    return vertexArrayID;
}

GLuint ModelManager::getProgramID()
{
    return programID;
}

void ModelManager::generateMVP(Model &model)
{
    ModelManager::projectionMatrix = getProjectionMatrix();
    ModelManager::viewMatrix = getViewMatrix();
    ModelManager::MVP = projectionMatrix * viewMatrix * model.getModelMatrix();
}

glm::mat4 ModelManager::getprojectionMatrix()
{
    return ModelManager::projectionMatrix;
}

glm::mat4 ModelManager::getviewMatrix()
{
    return ModelManager::viewMatrix;
}

GLuint ModelManager::getMatrixID()
{
    return ModelManager::matrixID;
}

GLuint ModelManager::getViewMatrixID()
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

void ModelManager::initializeMesh(Mesh &mesh)
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

void ModelManager::sendTransformation(Model &model)
{
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(model.getModelMatrixID(), 1, GL_FALSE, &model.getModelMatrix()[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
}

void ModelManager::Draw()
{
    glDrawElements(
            GL_TRIANGLES,        // mode
            mesh.getIndices()->size(),      // count
            GL_UNSIGNED_SHORT,   // type
            (void*)0             // element array buffer offset
    );
}

void ModelManager::createModel(Mesh & mesh, char* texturePath, char * textureSampler)
{
    models.push_back(Model(texturePath, textureSampler, this->programID, mesh.getId()));
}

GLuint modelManager::getLightID()
{
    return lightID;
}

void modelManager::setLightID(GLuint newID)
{
    modelManager::lightID = newID;
}

void modelManager::setProgramID(GLuint newID)
{
    modelManager::programID = newID;
}

void modelManager::setMatrixID(GLuint newID)
{
    modelManager::matrixID = newID;
}

void modelManager::setViewMatrixID(GLuint newID)
{
    modelManager::viewMatrixID = newID;
}

void modelManager::unloadMesh(Mesh &mesh)
{
    mesh.unloadMesh();
}

void modelManager::assignMatrix(Model &)
{

}

std::vector<Mesh> *modelManager::getMeshes()
{
    return &meshes;
}

std::vector<Model> *modelManager::getModels()
{
    return &models;
}



