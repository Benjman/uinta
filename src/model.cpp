#include <glm/vec3.hpp>
#include <uinta/logging.hpp>
#include <uinta/model.hpp>
#include <vector>

using namespace uinta;

const char* const uinta::getObjPath(const Models model) {
  switch (model) {
    case Model_Cube:
      return "model/cube.obj";
    case Model_Cone_LowPoly:
      return "model/cone_lowpoly.obj";
    case Model_Cylinder_LowPoly:
      return "model/cylinder_lowPoly.obj";
    case Model_Plane:
      return "model/plane.obj";
    case Model_Pyramid:
      return "model/pyramid.obj";
    case Model_Suzanne:
      return "model/suzanne.obj";
    case Model_Tri:
      return "model/tri.obj";
    default:
      throw std::runtime_error("Object path not defined for model\n");
  }
}

/***********************************/
/** Internal loading of obj files **/
/***********************************/

namespace uinta {
struct objface {
  int vert, uv, norm, index;

  objface() {
    vert = -1;
    uv = -1;
    norm = -1;
    index = -1;
  }

  bool operator==(const objface& other) {
    return vert == other.vert && uv == other.uv && norm == other.norm;
  }
};

void extractLineFloats(const std::vector<std::string>& lines, float* const buffer, const uint size, const char delimiter);
int findOrInsertFaceData(const objface&, std::vector<objface>&, const uint);
void packColors(const MeshAttrib&, float* const, uint* cont, const std::vector<objface>&, const float* const);
void packNormals(const MeshAttrib&, float* const, uint* cont, const std::vector<objface>&, const float* const);
void packUVs(const MeshAttrib&, float* const, uint* cont, const std::vector<objface>&, const float* const);
void packVertices(const MeshAttrib&, float* const, uint* cont, const std::vector<objface>&, const float* const, bool);
void parseFile(const std::string&, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&,
               std::vector<std::string>&);
void processFaceStrs(std::string*, std::vector<objface>&);
void processFaces(const std::vector<std::string>&, std::vector<objface>&, uint* const, uint* const);

}  // namespace uinta

void uinta::loadObj(const std::string& objBuffer, float* const vbuf, uint* vcount, uint* const ibuf, uint* icount,
                    uint* const ioff, const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  if (!attribs.size()) {
    SPDLOG_WARN("Unable to parse .obj file: No attributes provided!");
    return;
  }

  std::vector<std::string> vertexLines;
  std::vector<std::string> uvLines;
  std::vector<std::string> normalLines;
  std::vector<std::string> faceLines;

  parseFile(objBuffer, vertexLines, uvLines, normalLines, faceLines);

  *icount += faceLines.size() * 3;

  bool hasColor = vertexLines.size() > 0 && 5 == std::count(vertexLines.at(0).begin(), vertexLines.at(0).end(), ' ');

  float vertices[vertexLines.size() * (hasColor ? 6 : 3)];
  extractLineFloats(vertexLines, vertices, hasColor ? 6 : 3, ' ');

  float uvs[uvLines.size() * 2];
  extractLineFloats(uvLines, uvs, 2, ' ');

  float normals[normalLines.size() * 3];
  extractLineFloats(normalLines, normals, 3, ' ');

  std::vector<objface> faceData;
  processFaces(faceLines, faceData, ibuf, ioff);

  if (hasMeshAttrib(MeshAttribType_Normal, attribs)) {
    packNormals(findMeshAttrib(MeshAttribType_Normal, attribs), vbuf, vcount, faceData, normals);
  }
  if (hasMeshAttrib(MeshAttribType_UV, attribs)) {
    packUVs(findMeshAttrib(MeshAttribType_UV, attribs), vbuf, vcount, faceData, uvs);
  }
  if (hasMeshAttrib(MeshAttribType_Position, attribs)) {
    packVertices(findMeshAttrib(MeshAttribType_Position, attribs), vbuf, vcount, faceData, vertices, hasColor);
  }
  if (hasMeshAttrib(MeshAttribType_Color, attribs)) {
    if (!hasColor) {
      SPDLOG_WARN("Mesh color data was requested, but the `.obj` file does not contain color data!");
      return;
    }
    packColors(findMeshAttrib(MeshAttribType_Color, attribs), vbuf, vcount, faceData, vertices);
  }
}

void uinta::processFaces(const std::vector<std::string>& faceLines, std::vector<objface>& result, uint* const indexbuffer,
                         uint* const ioff) {
  for (uint i = 0, len = faceLines.size(); i < len; i++) {
    auto tmp = std::string(faceLines.at(i));

    std::string strs[3];
    strs[0] = tmp.substr(0, tmp.find(' '));
    tmp.erase(0, tmp.find(' ') + 1);
    strs[1] = tmp.substr(0, tmp.find(' '));
    tmp.erase(0, tmp.find(' ') + 1);
    strs[2] = tmp.substr(0, tmp.find(' '));

    processFaceStrs(strs, result);

    indexbuffer[i * 3 + 0] = findOrInsertFaceData(result[i * 3 + 0], result, len * 3) + *ioff;
    indexbuffer[i * 3 + 1] = findOrInsertFaceData(result[i * 3 + 1], result, len * 3) + *ioff;
    indexbuffer[i * 3 + 2] = findOrInsertFaceData(result[i * 3 + 2], result, len * 3) + *ioff;
  }

  *ioff += faceLines.size() * 3;
}

void uinta::packColors(const MeshAttrib& attrib, float* const vbuf, uint* const vcount, const std::vector<objface>& faceData,
                       const float* const vertexData) {
  for (int i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).vert - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &vertexData[index * 6 + 3], 3 * sizeof(float));
    *vcount += 3;
  }
}

void uinta::packNormals(const MeshAttrib& attrib, float* const vbuf, uint* const vcount, const std::vector<objface>& faceData,
                        const float* const normalData) {
  for (int i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).norm - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &normalData[index * 3], 3 * sizeof(float));
    *vcount += 3;
  }
}

void uinta::packUVs(const MeshAttrib& attrib, float* const vbuf, uint* const vcount, const std::vector<objface>& faceData,
                    const float* const uvData) {
  for (int i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).uv - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &uvData[index * 3], 3 * sizeof(float));
    *vcount += 3;
  }
}

void uinta::packVertices(const MeshAttrib& attrib, float* const vbuf, uint* const vcount, const std::vector<objface>& faceData,
                         const float* const vertexData, bool hasColor) {
  for (int i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).vert - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &vertexData[index * (hasColor ? 6 : 3)], 3 * sizeof(float));
    *vcount += 3;
  }
}

int uinta::findOrInsertFaceData(const objface& face, std::vector<objface>& faceData, const uint max) {
  for (int i = 0; i < max; i++) {
    if (faceData[i] == face) return i;
    if (faceData[i] == objface()) {
      faceData[i].vert = face.vert;
      faceData[i].uv = face.uv;
      faceData[i].norm = face.norm;
      return i;
    }
  }
  SPDLOG_WARN("Failed to find appropriate index. This is a \"critical\" warniung");
  return -1;
}

void uinta::processFaceStrs(std::string* values, std::vector<objface>& result) {
  for (int i = 0; i < 3; i++) {
    std::string tmp = std::string(values[i]);
    std::string vert = tmp.substr(0, tmp.find('/'));
    tmp.erase(0, tmp.find('/') + 1);
    std::string uv = tmp.substr(0, tmp.find('/'));
    tmp.erase(0, tmp.find('/') + 1);
    std::string norm = tmp.substr(0, tmp.find('/'));

    objface face;
    if (!vert.empty()) face.vert = std::stoi(vert);
    if (!uv.empty()) face.uv = std::stoi(uv);
    if (!norm.empty()) face.norm = std::stoi(norm);
    result.push_back(objface(face));
  }
}

/**
Reads an obj format file provided by parameter `objBuffer` line-by-line and extracts `v`, `vt`, `vn`, and `f` attributed data.
**/
void uinta::parseFile(const std::string& objBuffer, std::vector<std::string>& vertexLines, std::vector<std::string>& uvLines,
                      std::vector<std::string>& normalLines, std::vector<std::string>& faceLines) {
  std::string line;
  std::string flag;
  std::string data;

  // back and forward are cursors between new-lines.
  std::string::size_type back = 0;
  std::string::size_type forward = std::string::npos;

  while ((forward = objBuffer.find('\n', back)) != std::string::npos) {
    line = objBuffer.substr(back, forward - back);
    back = forward + 1;

    flag = line.substr(0, line.find(' '));
    data = line.substr(flag.length() + 1);

    if ("v" == flag) {
      vertexLines.push_back(std::string(data));
    } else if ("vt" == flag) {
      uvLines.push_back(std::string(data));
    } else if ("vn" == flag) {
      normalLines.push_back(std::string(data));
    } else if ("f" == flag) {
      faceLines.push_back(std::string(data));
    }
  }
}

void uinta::extractLineFloats(const std::vector<std::string>& lines, float* const buffer, const uint size, const char delimiter) {
  for (int i = 0, len = lines.size(); i < len; i++) {
    auto tmp = std::string(lines.at(i));
    for (int j = 0; j < size; j++) {
      buffer[i * size + j] = std::stof(tmp.substr(0, tmp.find(delimiter)));
      tmp.erase(0, tmp.find(delimiter) + 1);
    }
  }
}
