#ifndef UINTA_MODEL_HPP
#define UINTA_MODEL_HPP

#include <string>

#include <cstdio>
#include <unordered_map>

#include <mesh.hpp>

// TODO this all needs to be namespaced

enum Models {
    Model_Cube,
    Model_Plane,
    Model_Pyramid,
    Model_Suzanne,
    Model_Tri,
};

const char* const getObjPath(const Models);
void loadObj(const Models model, float* const vbuf, unsigned int* const ibuf, unsigned int* icount, const std::unordered_map<MeshAttribType, mesh_attrib>* const attribs);
// TODO a way to load obj files without it being specified as a `Models` type

#endif // UINTA_MODEL_HPP
