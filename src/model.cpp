#include <uinta/file.hpp>
#include <uinta/logging.hpp>
#include <uinta/model.hpp>

#include <vector>

#include <glm/vec3.hpp>
#include <spdlog/spdlog.h>

using namespace uinta;

const char *const uinta::getObjPath(const Models model) {
  switch (model) {
  case Model_Cube:
    return "model/cube.obj";
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
    vert  = -1;
    uv    = -1;
    norm  = -1;
    index = -1;
  }

  bool operator==(const objface &other) { return vert == other.vert && uv == other.uv && norm == other.norm; }
};

int findOrInsertFaceData(const objface &, std::vector<objface> &, const unsigned int);
void packNormals(const MeshAttrib &, float *const, unsigned int *cont, const std::vector<objface> &, const float *const);
void packUVs(const MeshAttrib &, float *const, unsigned int *cont, const std::vector<objface> &, const float *const);
void packVertices(const MeshAttrib &, float *const, unsigned int *cont, const std::vector<objface> &, const float *const);
void parseFile(std::string &, std::vector<std::string> &, std::vector<std::string> &, std::vector<std::string> &,
               std::vector<std::string> &);
void extractLineFloats(const std::vector<std::string> &lines, float *const buffer, const unsigned int size, const char delimiter);
void processFaceStrs(std::string *, std::vector<objface> &, unsigned int);
void processFaces(const std::vector<std::string> &, std::vector<objface> &, unsigned int *const);

} // namespace uinta

void uinta::loadObj(const Models model, float *const vbuf, unsigned int *vcount, unsigned int *const ibuf, unsigned int *icount,
                    const std::unordered_map<MeshAttribType, MeshAttrib> *const attribs) {
  if (!attribs->size()) {
    SPDLOG_WARN("Unable to parse .obj file: No attributes provided!");
    return;
  }

  char buf[getFileSize(getObjPath(model))];
  readFileRaw(getObjPath(model), buf);
  std::string bufstr = buf;

  std::vector<std::string> vertex_lines;
  std::vector<std::string> uv_lines;
  std::vector<std::string> normal_lines;
  std::vector<std::string> face_lines;

  parseFile(bufstr, vertex_lines, uv_lines, normal_lines, face_lines);

  *icount += face_lines.size() * 3;

  float vertices[vertex_lines.size() * 3];
  extractLineFloats(vertex_lines, vertices, 3, ' ');

  float uvs[uv_lines.size() * 2];
  extractLineFloats(uv_lines, uvs, 2, ' ');

  float normals[normal_lines.size() * 3];
  extractLineFloats(normal_lines, normals, 3, ' ');

  std::vector<objface> face_data;
  processFaces(face_lines, face_data, ibuf);

  if (const MeshAttrib *attrib = findMeshAttrib(MeshAttribType_Normal, attribs))
    packNormals(*attrib, vbuf, vcount, face_data, normals);
  if (const MeshAttrib *attrib = findMeshAttrib(MeshAttribType_UV, attribs))
    packUVs(*attrib, vbuf, vcount, face_data, uvs);
  if (const MeshAttrib *attrib = findMeshAttrib(MeshAttribType_Position, attribs))
    packVertices(*attrib, vbuf, vcount, face_data, vertices);
}

void uinta::processFaces(const std::vector<std::string> &face_lines, std::vector<objface> &result,
                         unsigned int *const indexbuffer) {
  /*
      Face lines describe a single triangle in the format of:
          `f {vert}/{uv}/{norm} {vert}/{uv}/{norm} {vert}/{uv}/{norm}`

      As an example:
          `f 1/2/3 4/5/6 7/8/9`
  */
  for (unsigned int i = 0, len = face_lines.size(); i < len; i++) {
    std::string tmp = std::string(face_lines.at(i));

    std::string strs[3];
    strs[0] = tmp.substr(0, tmp.find(' '));
    tmp.erase(0, tmp.find(' ') + 1);
    strs[1] = tmp.substr(0, tmp.find(' '));
    tmp.erase(0, tmp.find(' ') + 1);
    strs[2] = tmp.substr(0, tmp.find(' '));

    processFaceStrs(strs, result, i * 3);

    indexbuffer[i * 3 + 0] = findOrInsertFaceData(result[i * 3 + 0], result, len * 3);
    indexbuffer[i * 3 + 1] = findOrInsertFaceData(result[i * 3 + 1], result, len * 3);
    indexbuffer[i * 3 + 2] = findOrInsertFaceData(result[i * 3 + 2], result, len * 3);
  }
}

void uinta::packNormals(const MeshAttrib &attrib, float *const vbuf, unsigned int *const vcount,
                        const std::vector<objface> &face_data, const float *const normal_data) {
  for (int i = 0, len = face_data.size(); i < len; i++) {
    const unsigned int index = face_data.at(i).norm - 1;
    if (index < 0)
      break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &normal_data[index * 3], 3 * sizeof(float));
    *vcount += 3;
  }
}

void uinta::packUVs(const MeshAttrib &attrib, float *const vbuf, unsigned int *const vcount,
                    const std::vector<objface> &face_data, const float *const uv_data) {
  for (int i = 0, len = face_data.size(); i < len; i++) {
    const unsigned int index = face_data.at(i).uv - 1;
    if (index < 0)
      break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &uv_data[index * 3], 3 * sizeof(float));
    *vcount += 3;
  }
}

void uinta::packVertices(const MeshAttrib &attrib, float *const vbuf, unsigned int *const vcount,
                         const std::vector<objface> &face_data, const float *const vertex_data) {
  for (int i = 0, len = face_data.size(); i < len; i++) {
    const unsigned int index = face_data.at(i).vert - 1;
    if (index < 0)
      break;
    memcpy(&vbuf[attrib.offset + attrib.stride * i], &vertex_data[index * 3], 3 * sizeof(float));
    *vcount += 3;
  }
}

int uinta::findOrInsertFaceData(const objface &face, std::vector<objface> &face_data, const unsigned int max) {
  for (int i = 0; i < max; i++) {
    if (face_data[i] == face)
      return i;
    if (face_data[i] == objface()) {
      face_data[i].vert = face.vert;
      face_data[i].uv   = face.uv;
      face_data[i].norm = face.norm;
      return i;
    }
  }
  SPDLOG_WARN("Failed to find appropriate index. This is a \"critical\" warniung");
  return -1;
}

void uinta::processFaceStrs(std::string *values, std::vector<objface> &result, unsigned int index) {
  for (int i = 0; i < 3; i++) {
    std::string tmp  = std::string(values[i]);
    std::string vert = tmp.substr(0, tmp.find('/'));
    tmp.erase(0, tmp.find('/') + 1);
    std::string uv = tmp.substr(0, tmp.find('/'));
    tmp.erase(0, tmp.find('/') + 1);
    std::string norm = tmp.substr(0, tmp.find('/'));

    objface face;
    if (!vert.empty())
      face.vert = std::stoi(vert);
    if (!uv.empty())
      face.uv = std::stoi(uv);
    if (!norm.empty())
      face.norm = std::stoi(norm);
    result.push_back(objface(face));
  }
}

/**
Reads an obj format file provided by parameter `objBuffer` line-by-line and extracts `v`, `vt`, `vn`, and `f` attributed data.
**/
void uinta::parseFile(std::string &objBuffer, std::vector<std::string> &vertex_lines, std::vector<std::string> &uv_lines,
                      std::vector<std::string> &normal_lines, std::vector<std::string> &face_lines) {
  std::string line;
  std::string flag;
  std::string data;

  // back and forward are cursors between new-lines.
  std::string::size_type back    = 0;
  std::string::size_type forward = std::string::npos;

  while ((forward = objBuffer.find('\n', back)) != std::string::npos) {
    line = objBuffer.substr(back, forward - back);
    back = forward + 1;

    flag = line.substr(0, line.find(' '));
    data = line.substr(flag.length() + 1);

    if ("v" == flag) {
      vertex_lines.push_back(std::string(data));
    } else if ("vt" == flag) {
      uv_lines.push_back(std::string(data));
    } else if ("vn" == flag) {
      normal_lines.push_back(std::string(data));
    } else if ("f" == flag) {
      face_lines.push_back(std::string(data));
    }
  }
}

void uinta::extractLineFloats(const std::vector<std::string> &lines, float *const buffer, const unsigned int size,
                              const char delimiter) {
  for (int i = 0, len = lines.size(); i < len; i++) {
    std::string tmp = std::string(lines.at(i));
    for (int j = 0; j < size; j++) {
      buffer[i * size + j] = std::stof(tmp.substr(0, tmp.find(delimiter)));
      tmp.erase(0, tmp.find(delimiter) + 1);
    }
  }
}
