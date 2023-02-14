#ifndef UINTA_OBJ_HPP
#define UINTA_OBJ_HPP

#include <string>
#include <uinta/mesh.hpp>

namespace uinta {

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

#endif  // UINTA_OBJ_HPP
