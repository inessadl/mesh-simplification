//
// Created by inessa on 13/04/16.
//

#include <modelmanager.hpp>
#include <shader.hpp>
#include <iostream>
#include <controls.hpp>
#include <glm/gtc/matrix_transform.hpp>

void ModelManager::ModelManager (char* vertexShaders, char* fragmentShaders, char* lightPosition)
{
    glGenerateVertextArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    programID = LoadShaders(vertexShaders, fragmentShaders);
    lightID = glGetUniformLocation(programID, lightPosition);
    matrixID = glGetUniformLocation(programID, "MVP");
    viewMatrixID = glGetUniformLocation(programID, "V");
}

/* Default destructor */
ModelManager::~ModelManager()
{

}

/* Getters */
std::vector<Mesh> *ModelManager::getMeshes()    {  return &meshes;  }
std::vector<Model> *ModelManager::getModels()   {  return &models;  }

GLuint ModelManager::getVertexArrayID()         {  return vertexArrayID;  }
GLuint ModelManager::getProgramID()             {  return programID;  }
GLuint ModelManager::getMatrixID()              {  return ModelManager::matrixID;   }
GLuint ModelManager::getViewMatrixID()          {  return ModelManager::viewMatrixID;   }
GLuint ModelManager::getLightID()               {  return lightID;  }

glm::mat4 ModelManager::getprojectionMatrix()   {  return ModelManager::projectionMatrix;   }
glm::mat4 ModelManager::getviewMatrix()         {  return ModelManager::viewMatrix;   }


/* Setters */
void ModelManager::setProgramID(GLuint id)      {  modelManager::programID = id; }
void ModelManager::setMatrixID(GLuint id)       {  modelManager::matrixID = id; }
void ModelManager::setViewMatrixID(GLuint id)   {  modelManager::viewMatrixID = id;}
void ModelManager::setLightID(GLuint id)        {  modelManager::lightID = id;  }

void ModelManager::setProjectionMatrix(glm::mat4 projectionMatrix)  {   ModelManager::projectionMatrix = projectionMatrix;  }
void ModelManager::setViewMatrix(glm::mat4 viewMatrix)              {   ModelManager::viewMatrix = viewMatrix;  }

void ModelManager::setLightPosition(glm::vec3 lightPos = glm::vec3(4, 4, 4))
{
    glUniform3f(lightID, lightPos.x, lightPos.y, lightPos.z);
}


void ModelManager::generateMVP(Model &model)
{
    ModelManager::projectionMatrix = getProjectionMatrix();
    ModelManager::viewMatrix = getViewMatrix();
    ModelManager::MVP = projectionMatrix * viewMatrix * model.getModelMatrix();
}

void ModelManager::sendTransformation(Model &model)
{
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(model.getModelMatrixID(), 1, GL_FALSE, &model.getModelMatrix()[0][0]);
    glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
}

void ModelManager::createModel(Mesh & mesh, char* texturePath, char * textureSampler)
{
    models.push_back(Model(texturePath, textureSampler, this->programID, &mesh));
}

void ModelManager::Draw()
{
  models[0].setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(-1.0f, 0.0f, -3.0f)));
  models[1].setModelMatrix(glm::translate(glm::mat4(1.0), glm::vec3(1.0f, 0.0f, 3.0f)));

  for (int j = 0; j < models.size(); j++)
  {
    generateMVP(models[j]);
    sendTransformation(models[j]);
    models[j].draw();
  }
}


void ModelManager::loadMesh(char* path)
{
    ModelManager::meshes.push_back(Mesh(path));
}

void ModelManager::unloadMesh(Mesh &mesh)
{
    mesh.unloadMesh();
}

void ModelManager::assignMatrix(Model &)
{

}
