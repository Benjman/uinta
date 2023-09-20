#ifndef UINTA_OBJ_HPP
#define UINTA_OBJ_HPP

#include <string>
#include <uinta/fwd.hpp>
#include <uinta/mesh.hpp>

namespace uinta {

// USAGE
//
//  Extract vertex and index data from a provided `.obj` buffer.
//      std::string buffer = "# obj file definition";
//      f32 vertices[vertexBufferSize];
//      u32 indices[indexBufferSize];
//      u32 ioff = 0;
//      u32 vcount = 0;
//      loadObj(fileManager.getDataString(file), vertices, &vcount, indices, &icount, &ioff,
//              {
//                  {MeshAttribType_Position, {9, 0}},
//                  {MeshAttribType_Normal, {9, 3}},
//                  {MeshAttribType_Color, {9, 6}},
//              });
uinta_error_code loadObj(const std::string& buffer, f32* const vbuf, u32* const vcount, u32* const ibuf, u32* icount,
                         u32* const ioff, const std::unordered_map<MeshAttribType, MeshAttrib>& attribs);

}  // namespace uinta

#endif  // UINTA_OBJ_HPP
