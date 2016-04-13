//
// Created by inessa on 12/04/16.
//

#ifndef CG_UFPEL_MESH_H
#define CG_UFPEL_MESH_H


class Mesh
{

private:
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;

  std::vector<unsigned short> indices;
  std::vector<glm::vec3> indexed_vertices;
  std::vector<glm::vec2> indexed_uvs;
  std::vector<glm::vec3> indexed_normals;

public:
  GLuint vertexbuffer;
  GLuint uvbuffer;
  GLuint normalbuffer;
  GLuint elementbuffer;

};


#endif //CG_UFPEL_MESH_H
