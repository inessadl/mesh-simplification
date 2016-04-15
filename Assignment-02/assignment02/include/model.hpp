//
// Created by inessa on 12/04/16.
//

#ifndef MODEL_H
#define MODEL_H

#include<vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Model
{
    glm::mat4 modelMatrix;
    GLuint modelMatrixID;
    GLuint texture;
    GLuint textureID;
    int meshID;
    std::vector<glm::mat4> animatedMatrix;

public:
    Model(char*, char*, GLuint, int);
    ~Model();

    // Getters
    glm::mat4 getModelMatrix() ;
    GLuint getModelMatrixID() ;
    GLuint getTexture() ;
    GLuint getTextureID() ;
    GLuint getMeshID() ;

    // Setters
    void setModelMatrixID(GLuint);
//    void setTexture(GLuint);
    void setTextureID(GLuint);

    void queueTransformation();
    void applyTransformation();
    void interpolateMatrix();
    void bindTexture();

};


#endif //CG_UFPEL_MODEL_H
