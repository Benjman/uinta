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
void loadObj(const std::string& buffer, float* const vbuf, uint32_t* const vcount, uint32_t* const ibuf, uint32_t* icount,
             uint32_t* const ioff, const std::unordered_map<MeshAttribType, MeshAttrib>* const attribs);
}  // namespace uinta

#endif  // UINTA_MODEL_HPP
