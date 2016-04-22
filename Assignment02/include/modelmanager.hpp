//
// Created by inessa on 20/04/16.
//

#ifndef ASSIGNMENT02_MODELMANAGER_H
#define ASSIGNMENT02_MODELMANAGER_H

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

    /* Getters */
    GLuint getMatrixID();
    GLuint getViewMatrixID();

    glm::mat4 getprojectionMatrix();
    glm::mat4 getViewMatrix();

    GLuint getvertexArrayID();
    GLuint getProgramID();
    GLuint getLightID();

    std::vector<Mesh> * getMeshes();
    std::vector<Model> * getModels();

    /* Setters */
    void setprojectionMatrix(glm::mat4);
    void setViewMatrix(glm::mat4);
    void setLightID(GLuint);
    void setProgramID(GLuint);
    void setMatrixID(GLuint);
    void setViewMatrixID(GLuint);
    void setLightPosition(glm::vec3);

    void createModel(Mesh &, char *, char *);
    void assignMatrix(Model &);

    void loadMesh(char *);
    void unloadMesh(Mesh &);

    void draw();

};


#endif //ASSIGNMENT02_MODELMANAGER_H
