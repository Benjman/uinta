#ifndef UINTA_MODEL_HPP
#define UINTA_MODEL_HPP

#include <string>
#include <uinta/mesh.hpp>

namespace uinta {

enum Models {
  Model_Cube,
  Model_Cone_LowPoly,
  Model_Cylinder_LowPoly,
  Model_Plane,
  Model_Pyramid,
  Model_Suzanne,
  Model_Tri,
};

const char* const getObjPath(const Models);

// USAGE
//
//  Extract vertex and index data from a provided `.obj` buffer.
//      std::string buffer = "# obj file definition";
//      float vertices[vertexBufferSize];
//      uint indices[indexBufferSize];
//      uint ioff = 0;
//      uint vcount = 0;
//      loadObj(fileManager.getDataString(file), vertices, &vcount, indices, &icount, &ioff,
//              {
//                  {MeshAttribType_Position, {9, 0}},
//                  {MeshAttribType_Normal, {9, 3}},
//                  {MeshAttribType_Color, {9, 6}},
//              });
void loadObj(const std::string& buffer, float* const vbuf, uint32_t* const vcount, uint32_t* const ibuf, uint32_t* icount,
             uint32_t* const ioff, const std::unordered_map<MeshAttribType, MeshAttrib>& attribs);

}  // namespace uinta

#endif  // UINTA_MODEL_HPP
