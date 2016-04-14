//
// Created by inessa on 12/04/16.
//

#ifndef CG_UFPEL_MODEL_H
#define CG_UFPEL_MODEL_H


class Model
{
  glm::mat4 ModelMatrix;
  GLuint texture;
  GLuint textureID;
  const int meshID;

public:
  glm::mat4 getModelMatrix() const;
  GLuint getTexture() const;
  GLuint getTextureID() const;
  void queueTransformation();
  void applyTransformation();
  void bindTexture();
};


#endif //CG_UFPEL_MODEL_H
