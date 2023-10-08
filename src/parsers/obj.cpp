#include <glm/vec3.hpp>
#include <uinta/error.hpp>
#include <uinta/logging.hpp>
#include <uinta/parsers/obj.hpp>
#include <vector>

using namespace uinta;

/***********************************/
/** Internal loading of obj files **/
/***********************************/

namespace uinta {

enum class error : u8 {
  AttribsMissing = 10,
  ColorsMissing = 11,
};
static const std::map<uinta_error_code_t, std::string> errorMessages = {
    {static_cast<uinta_error_code_t>(error::AttribsMissing), "No attributes provided!"},
    {static_cast<uinta_error_code_t>(error::ColorsMissing), "Color data requested but missing from .obj file!"},
};

UINTA_ERROR_FRAMEWORK(ObjParser, errorMessages);

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

uinta_error_code uinta::loadObj(const std::string& objBuffer, f32* const vbuf, u32* vcount, u32* const ibuf, u32* icount,
                                u32* const ioff, const std::unordered_map<MeshAttribType, MeshAttrib>& attribs) {
  if (attribs.size() == 0) return make_error(error::AttribsMissing);

  std::vector<std::string> vertexLines;
  std::vector<std::string> uvLines;
  std::vector<std::string> normalLines;
  std::vector<std::string> faceLines;

  parseFile(objBuffer, vertexLines, uvLines, normalLines, faceLines);

  *icount += faceLines.size() * 3;

  static constexpr char DELIM = ' ';

  bool hasColor = vertexLines.size() > 0 && 5 == std::count(vertexLines.at(0).begin(), vertexLines.at(0).end(), DELIM);

  f32 vertices[vertexLines.size() * (hasColor ? 6 : 3)];
  extractLineFloats(vertexLines, vertices, hasColor ? 6 : 3, DELIM);

  f32 uvs[uvLines.size() * 2];
  extractLineFloats(uvLines, uvs, 2, DELIM);

  f32 normals[normalLines.size() * 3];
  extractLineFloats(normalLines, normals, 3, DELIM);

  std::vector<objface> faceData;
  processFaces(faceLines, faceData, ibuf, ioff);

  if (hasMeshAttrib(MeshAttribType::Normal, attribs)) {
    packNormals(findMeshAttrib(MeshAttribType::Normal, attribs), vbuf, vcount, faceData, normals);
  }
  if (hasMeshAttrib(MeshAttribType::UV, attribs)) {
    packUVs(findMeshAttrib(MeshAttribType::UV, attribs), vbuf, vcount, faceData, uvs);
  }
  if (hasMeshAttrib(MeshAttribType::Position, attribs)) {
    packVertices(findMeshAttrib(MeshAttribType::Position, attribs), vbuf, vcount, faceData, vertices, hasColor);
  }
  if (hasMeshAttrib(MeshAttribType::Color, attribs)) {
    if (!hasColor) return make_error(error::ColorsMissing);
    packColors(findMeshAttrib(MeshAttribType::Color, attribs), vbuf, vcount, faceData, vertices);
  }
  return SUCCESS_EC;
}

void uinta::processFaces(const std::vector<std::string>& faceLines, std::vector<objface>& result, u32* const indexbuffer,
                         u32* const ioff) {
  for (u32 i = 0, len = faceLines.size(); i < len; i++) {
    auto tmp = std::string(faceLines.at(i));

    static constexpr char DELIM = ' ';
    std::string strs[3];
    strs[0] = tmp.substr(0, tmp.find(DELIM));
    tmp.erase(0, tmp.find(DELIM) + 1);
    strs[1] = tmp.substr(0, tmp.find(DELIM));
    tmp.erase(0, tmp.find(DELIM) + 1);
    strs[2] = tmp.substr(0, tmp.find(DELIM));

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
  SPDLOG_WARN("Failed to find matching index!");
  return -1;
}

void uinta::processFaceStrs(std::string* values, std::vector<objface>& result) {
  static constexpr char DELIM = '/';
  for (i32 i = 0; i < 3; i++) {
    std::string tmp = std::string(values[i]);
    std::string vert = tmp.substr(0, tmp.find(DELIM));
    tmp.erase(0, tmp.find(DELIM) + 1);
    std::string uv = tmp.substr(0, tmp.find(DELIM));
    tmp.erase(0, tmp.find(DELIM) + 1);
    std::string norm = tmp.substr(0, tmp.find(DELIM));

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

    static constexpr std::string VERTEX_TOKEN = "v";
    static constexpr std::string UV_TOKEN = "vt";
    static constexpr std::string NORMAL_TOKEN = "vn";
    static constexpr std::string FACE_TOKEN = "f";

    if (VERTEX_TOKEN == flag) {
      vertexLines.push_back(std::string(data));
    } else if (UV_TOKEN == flag) {
      uvLines.push_back(std::string(data));
    } else if (NORMAL_TOKEN == flag) {
      normalLines.push_back(std::string(data));
    } else if (FACE_TOKEN == flag) {
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
