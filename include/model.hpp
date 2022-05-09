#ifndef UINTA_MODEL_HPP
#define UINTA_MODEL_HPP

#include <string>

#include <cstdio>
#include <unordered_map>

#include <mesh.hpp>

enum Models {
    Model_Cube,
    Model_Pyramid,
    Model_Tri,
};

const unsigned int getObjFileSize(const Models);
const char* const getObjPath(const Models);
void loadObj(const Models model, float* vbuf, unsigned int* ibuf, const std::unordered_map<MeshAttribType, mesh_attrib>* attribs, const unsigned int attrib_count);

#endif // UINTA_MODEL_HPP



#include <stdexcept>
const char* const getObjPath(const Models model) {
    switch (model) {
        case Model_Cube: return "model/cube.obj";
        case Model_Pyramid: return "model/pyramid.obj";
        case Model_Tri: return "model/tri.obj";
        default: throw std::runtime_error("Object path not defined for model\n");
    }
}

const unsigned int getObjFileSize(const Models model) {
    switch (model) {
        case Model_Cube: return 656u;
        case Model_Pyramid: return 360u;
        case Model_Tri: return 202u;
        default: throw std::runtime_error("Object size not defined for model\n");
    }
}

#include <vector>
void packObjData(const mesh_attrib& attrib, float* vbuf, const std::vector<std::string>& data) {
    std::string::size_type left = 0;
    std::string::size_type right = std::string::npos;
    for (int i = 0; i < data.size(); i++) {
        std::string tmp = std::string(data.at(i));
        float x = std::stof(tmp.substr(0, tmp.find(' ')));
        tmp.erase(0, tmp.find(' ') + 1);
        float y = std::stof(tmp.substr(0, tmp.find(' ')));
        tmp.erase(0, tmp.find(' ') + 1);
        float z = std::stof(tmp.substr(0, tmp.find(' ')));

        vbuf[attrib.offset + attrib.stride * i + 0] = x;
        vbuf[attrib.offset + attrib.stride * i + 1] = y;
        vbuf[attrib.offset + attrib.stride * i + 2] = z;
    }
}

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

int findOrInsertFaceDataIndex(const objface& face, objface* const facedata, const unsigned int max) {
    for (int i = 0; i < max; i++) {
        if (facedata[i] == face)
            return i;

        if (facedata[i] == objface()) {
            facedata[i].vert = face.vert;
            facedata[i].uv = face.uv;
            facedata[i].norm = face.norm;
            return i;
        }
    }

    return -1;
}

void processFaceStrs(std::string* v, objface* const result) {
    for (int i = 0; i < 3; i++) {
        std::string tmp = std::string(v[i]);
        std::string vertstr = tmp.substr(0, tmp.find('/'));
        tmp.erase(0, tmp.find('/') + 1);
        std::string uvstr = tmp.substr(0, tmp.find('/'));
        tmp.erase(0, tmp.find('/') + 1);
        std::string normstr = tmp.substr(0, tmp.find('/'));

        if (!vertstr.empty()) result[i].vert = std::stoi(vertstr);
        if (!uvstr.empty())   result[i].uv = std::stoi(uvstr);
        if (!normstr.empty()) result[i].norm = std::stoi(normstr);
    }
}

void processFaces(const std::vector<std::string>& data, objface* const facedata, unsigned int* const indexbuffer) {
    /*
        Face lines describe a single triangle in the format of:
            `f {vert}/{uv}/{norm} {vert}/{uv}/{norm} {vert}/{uv}/{norm}`

        As an example:
            `f 1/2/3 4/5/6 7/8/9`
    */

    unsigned short indices[data.size() * 3];

    for (unsigned int i = 0, len = data.size(); i < len; i++) {
        std::string tmp = std::string(data.at(i));

        std::string strs[3];
        strs[0] = tmp.substr(0, tmp.find(' '));
        tmp.erase(0, tmp.find(' ') + 1);
        strs[1] = tmp.substr(0, tmp.find(' '));
        tmp.erase(0, tmp.find(' ') + 1);
        strs[2] = tmp.substr(0, tmp.find(' '));

        processFaceStrs(strs, &facedata[i * 3]);

        indexbuffer[i * 3 + 0] = findOrInsertFaceDataIndex(facedata[i * 3 + 0], facedata, len * 3);
        indexbuffer[i * 3 + 1] = findOrInsertFaceDataIndex(facedata[i * 3 + 1], facedata, len * 3);
        indexbuffer[i * 3 + 2] = findOrInsertFaceDataIndex(facedata[i * 3 + 2], facedata, len * 3);
    }
}

void processObjFile(std::string& buf, std::vector<std::string>& vertices, std::vector<std::string>& normals, std::vector<std::string>& faces) {
    std::string line;
    std::string flag;
    std::string data;

    // extract lines
    std::string::size_type left = 0;
    std::string::size_type right = std::string::npos;

    while ((right = buf.find('\n', left)) != std::string::npos) {
        line = buf.substr(left, right - left);
        left = right + 1;

        flag = line.substr(0, line.find(' '));
        data = line.substr(flag.length() + 1);

        if ("v" == flag) {
            vertices.push_back(std::string(data));
        } else if ("vn" == flag) {
            normals.push_back(std::string(data));
        } else if ("f" == flag) {
            faces.push_back(std::string(data));
        }
    }
}

#include <file.hpp>
#include <cstring> // for memset
void loadObj(const Models model, float* vbuf, unsigned int* ibuf, const std::unordered_map<MeshAttribType, mesh_attrib>* attribs, const unsigned int attrib_count) {
    if (!attribs->size()) {
        printf("[WARN] Unable to parse .obj file: No attributes provided");
        return;
    }

    std::vector<std::string> vertices;
    std::vector<std::string> normals;
    std::vector<std::string> faces;

    char buf[getObjFileSize(model)];
    read_file_raw(getObjPath(model), buf);
    std::string bufstr = buf;

    processObjFile(bufstr, vertices, normals, faces);

    unsigned int max_indices = faces.size() * 3;
    unsigned int indexbuffer[max_indices];
    memset(indexbuffer, 0, sizeof(indexbuffer));

    objface facedata[max_indices];

    processFaces(faces, facedata, indexbuffer);

    if (const mesh_attrib* attrib = find_mesh_attrib(MeshAttribType_Position, attribs))
        packObjData(*attrib, vbuf, vertices);
    if (const mesh_attrib* attrib = find_mesh_attrib(MeshAttribType_Normal, attribs))
        packObjData(*attrib, vbuf, normals);
}
