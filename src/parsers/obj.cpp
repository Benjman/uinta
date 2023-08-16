#include <glm/vec3.hpp>
#include <uinta/logging.hpp>
#include <uinta/parsers/obj.hpp>
#include <vector>

using namespace uinta;

/***********************************/
/** Internal loading of obj files **/
/***********************************/

namespace uinta {
struct objface {
  i32 vert, uv, norm, index;

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

void extractLineFloats(const std::vector<std::string>& lines, f32* const buffer, const u32 size, const char delimiter);
i32 findOrInsertFaceData(const objface&, std::vector<objface>&, const u32);
void packColors(const MeshAttrib&, f32* const, u32* cont, const std::vector<objface>&, const f32* const);
void packNormals(const MeshAttrib&, f32* const, u32* cont, const std::vector<objface>&, const f32* const);
void packUVs(const MeshAttrib&, f32* const, u32* cont, const std::vector<objface>&, const f32* const);
void packVertices(const MeshAttrib&, f32* const, u32* cont, const std::vector<objface>&, const f32* const, bool);
void parseFile(const std::string&, std::vector<std::string>&, std::vector<std::string>&, std::vector<std::string>&,
               std::vector<std::string>&);
void processFaceStrs(std::string*, std::vector<objface>&);
void processFaces(const std::vector<std::string>&, std::vector<objface>&, u32* const, u32* const);

}  // namespace uinta

void uinta::loadObj(const std::string& objBuffer, f32* const vbuf, u32* vcount, u32* const ibuf, u32* icount, u32* const ioff,
                    const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
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

  f32 vertices[vertexLines.size() * (hasColor ? 6 : 3)];
  extractLineFloats(vertexLines, vertices, hasColor ? 6 : 3, ' ');

  f32 uvs[uvLines.size() * 2];
  extractLineFloats(uvLines, uvs, 2, ' ');

  f32 normals[normalLines.size() * 3];
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

void uinta::processFaces(const std::vector<std::string>& faceLines, std::vector<objface>& result, u32* const indexbuffer,
                         u32* const ioff) {
  for (u32 i = 0, len = faceLines.size(); i < len; i++) {
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

void uinta::packColors(const MeshAttrib& attrib, f32* const vbuf, u32* const vcount, const std::vector<objface>& faceData,
                       const f32* const vertexData) {
  for (i32 i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).vert - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &vertexData[index * 6 + 3], 3 * sizeof(f32));
    *vcount += 3;
  }
}

void uinta::packNormals(const MeshAttrib& attrib, f32* const vbuf, u32* const vcount, const std::vector<objface>& faceData,
                        const f32* const normalData) {
  for (i32 i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).norm - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &normalData[index * 3], 3 * sizeof(f32));
    *vcount += 3;
  }
}

void uinta::packUVs(const MeshAttrib& attrib, f32* const vbuf, u32* const vcount, const std::vector<objface>& faceData,
                    const f32* const uvData) {
  for (i32 i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).uv - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &uvData[index * 3], 3 * sizeof(f32));
    *vcount += 3;
  }
}

void uinta::packVertices(const MeshAttrib& attrib, f32* const vbuf, u32* const vcount, const std::vector<objface>& faceData,
                         const f32* const vertexData, bool hasColor) {
  for (i32 i = 0, len = faceData.size(); i < len; i++) {
    auto index = faceData.at(i).vert - 1;
    if (index < 0) break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &vertexData[index * (hasColor ? 6 : 3)], 3 * sizeof(f32));
    *vcount += 3;
  }
}

i32 uinta::findOrInsertFaceData(const objface& face, std::vector<objface>& faceData, const u32 max) {
  static const objface EMPTY_FACE = {};
  for (u32 i = 0; i < max; i++) {
    if (faceData[i] == face) return i;
    if (faceData[i] == EMPTY_FACE) {
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
  for (i32 i = 0; i < 3; i++) {
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

  // back and forward are cursors between new-lines.
  std::string::size_type back = 0;
  std::string::size_type forward;

  while ((forward = objBuffer.find('\n', back)) != std::string::npos) {
    std::string flag;
    std::string data;

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

void uinta::extractLineFloats(const std::vector<std::string>& lines, f32* const buffer, const u32 size, const char delimiter) {
  for (size_t i = 0, len = lines.size(); i < len; i++) {
    auto tmp = std::string(lines.at(i));
    for (size_t j = 0; j < size; j++) {
      buffer[i * size + j] = std::stof(tmp.substr(0, tmp.find(delimiter)));
      tmp.erase(0, tmp.find(delimiter) + 1);
    }
  }
}
