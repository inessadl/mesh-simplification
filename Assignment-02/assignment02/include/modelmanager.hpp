//
// Created by inessa on 13/04/16.
//

#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <glm/glm.hpp>
#include <vector>
#include "model.hpp"
#include "mesh.hpp"

class ModelManager
{
    std::vector<Mesh> meshes;
    std::vector<Model> models;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 MVP;

    GLuint vertexArrayID;
    GLuint programID;
    GLuint matrixID;
    GLuint viewMatrixID;
    GLuint lightID;

    void generateMVP(Model&);
    void sendTransformation(Model&);
    void initializeMesh(Mesh&);

public:
    ModelManager(char*, char*, char*);
    ~ModelManager();

    // Getters
    GLuint getMatrixID();
    GLuint getViewMatrixID();

    glm::mat4 getprojectionMatrix();
    glm::mat4 getviewMatrix();

    GLuint getVertexArrayID();
    GLuint getProgramID();
    GLuint getLightID();

    std::vector<Mesh> * getMeshes();
    std::vector<Model> * getModels();

    // Setters
    void setProjectionMatrix(glm::mat4);
    void setViewMatrix(glm::mat4);
    void setLightID(GLuint);
    void setProgramID(GLuint);
    void setMatrixID();
    void setViewMatrixID();

    void setLightPosition(glm::vec3);
    // void activateTexture(Model &);
    void createModel(Mesh &, char *, char *);
    void loadMesh(char*);
    // void initializeMesh(Mesh&);
    // void unloadMesh(Mesh&);
    // void generateMVP(Model&);
    // void assignMatrix(Model&);
    // void sendTransformation(Model&);
    void draw();

};


#endif //MODELMANAGER_H
