#ifndef UINTA_MODEL_HPP
#define UINTA_MODEL_HPP

#include <uinta/mesh.hpp>

#include <cstdio>
#include <string>
#include <unordered_map>

namespace uinta {

enum Models {
  Model_Cube,
  Model_Dude,
  Model_Plane,
  Model_Pyramid,
  Model_Suzanne,
  Model_Tri,
};

const char *const getObjPath(const Models);
void loadObj(const Models model, float *const vbuf, unsigned int *const vcount, unsigned int *const ibuf, unsigned int *icount,
             const std::unordered_map<MeshAttribType, MeshAttrib> *const attribs);
// TODO a way to load obj files without it being specified as a `Models` type

} // namespace uinta

#endif // UINTA_MODEL_HPP
