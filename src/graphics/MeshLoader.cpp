#include "MeshLoader.hpp"

namespace mc {

static std::vector<glm::vec3> GetVertices(const std::string& data);
static std::vector<glm::vec2> GetUVs(const std::string& data);
static std::vector<glm::vec3> GetNormals(const std::string& data);
int LoadIndices(const std::string& data,
                std::vector<unsigned int>& vertexIndices,
                std::vector<unsigned int>& uvIndices,
                std::vector<unsigned int>& normalIndices);

VertexData::VertexData(const glm::vec3& vertex_,
                       const glm::vec2& uv_,
                       const glm::vec3& normal_):
    vertex { vertex_ },
    uv { uv_ },
    normal { normal_ } {
}

bool VertexData::operator<(const VertexData& other) const {
    for (int i = 0; i < 3; i++) {
        if (vertex[i] < other.vertex[i]) {
            return true;
        } else if (vertex[i] > other.vertex[i]) {
            return false;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (uv[i] < other.uv[i]) {
            return true;
        } else if (uv[i] > other.uv[i]) {
            return false;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (normal[i] < other.normal[i]) {
            return true;
        } else if (normal[i] > other.normal[i]) {
            return false;
        }
    }
    return false;
}

void LoadMeshDataFromFile(const std::string& filename,
                          std::vector<glm::vec3>& vertices,
                          std::vector<glm::vec2>& uvs,
                          std::vector<glm::vec3>& normals,
                          std::vector<uint32_t>& indices) {
    DEBUG("Loading mesh data from file \"", filename, "\"");

    std::string data = ReadFile(filename);

    std::vector<glm::vec3> verticesTmp = GetVertices(data);
    TRACE("Read ", verticesTmp.size(), " vertices");
    std::vector<glm::vec2> uvsTmp = GetUVs(data);
    TRACE("Read ", uvsTmp.size(), " UVs");
    std::vector<glm::vec3> normalsTmp = GetNormals(data);
    TRACE("Read ", normalsTmp.size(), " normals");

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    int count = LoadIndices(data, vertexIndices, uvIndices, normalIndices);
    TRACE("Read ", count, " faces");
    assert(vertexIndices.size() == uvIndices.size());
    assert(uvIndices.size() == normalIndices.size());

    std::map<VertexData, uint32_t> indexedVertices;
    for ( size_t i = 0; i < vertexIndices.size(); i++) {
        auto& vertex = verticesTmp[vertexIndices[i] - 1];
        auto& uv = uvsTmp[uvIndices[i] - 1];
        auto& normal = normalsTmp[normalIndices[i] - 1];
        VertexData newVertex(vertex, uv, normal);

        auto foundVertex = indexedVertices.find(newVertex);
        if (foundVertex != indexedVertices.end()) {
            TRACE("Adding already indexed vertex");
            indices.push_back(foundVertex->second);
        } else {
            TRACE("Adding new vertex");
            vertices.push_back(newVertex.vertex);
            uvs.push_back(newVertex.uv);
            normals.push_back(newVertex.normal);
            uint32_t index = static_cast<uint32_t>(vertices.size() - 1);
            indices.push_back(index);
            indexedVertices.emplace(newVertex, index);
        }
    }


    DEBUG("Loaded mesh data");
}

std::vector<glm::vec3> GetVertices(const std::string& data) {
    static const std::regex regEx("(?:^|\n)v ([-\\.[:digit:]]+) ([-\\.[:digit:]]+) ([-\\.[:digit:]]+)");

    std::vector<glm::vec3> vertices;
    std::smatch match;
    std::string::const_iterator start = data.cbegin();
    std::string::const_iterator stop = data.cend();

    while (std::regex_search(start, stop, match, regEx)) {
        glm::vec3 vertex;
        vertex.x = atof(match.str(1).c_str());
        vertex.y = atof(match.str(2).c_str());
        vertex.z = atof(match.str(3).c_str());
        vertices.push_back(vertex);

        start += match.position(0) + match.length(0);
    }
    return vertices;
}

std::vector<glm::vec2> GetUVs(const std::string& data) {
    static const std::regex regEx("(?:^|\n)vt ([-\\.[:digit:]]+) ([-\\.[:digit:]]+)");

    std::vector<glm::vec2> uvs;
    std::smatch match;
    std::string::const_iterator start = data.cbegin();
    std::string::const_iterator stop = data.cend();

    while (std::regex_search(start, stop, match, regEx)) {
        glm::vec2 uv;
        uv.x = atof(match.str(1).c_str());
        uv.y = atof(match.str(2).c_str());
        uvs.push_back(uv);

        start += match.position(0) + match.length(0);
    }
    return uvs;
}

std::vector<glm::vec3> GetNormals(const std::string& data) {
    static const std::regex regEx("(?:^|\n)vn ([-\\.[:digit:]]+) ([-\\.[:digit:]]+) ([-\\.[:digit:]]+)");

    std::vector<glm::vec3> normals;
    std::smatch match;
    std::string::const_iterator start = data.cbegin();
    std::string::const_iterator stop = data.cend();

    while (std::regex_search(start, stop, match, regEx)) {
        glm::vec3 normal;
        normal.x = atof(match.str(1).c_str());
        normal.y = atof(match.str(2).c_str());
        normal.z = atof(match.str(3).c_str());
        normals.push_back(normal);

        start += match.position(0) + match.length(0);
    }
    return normals;
}

int LoadIndices( const std::string& data,
                std::vector<unsigned int>& vertexIndices,
                std::vector<unsigned int>& uvIndices,
                std::vector<unsigned int>& normalIndices) {

    static const std::regex regEx("(?:^|\n)f (\\d+)/(\\d+)/(\\d+) (\\d+)/(\\d+)/(\\d+) (\\d+)/(\\d+)/(\\d+)");

    std::smatch match;
    int foundCount = 0;
    std::string::const_iterator start = data.cbegin();
    std::string::const_iterator stop = data.cend();

    while (std::regex_search(start, stop, match, regEx)) {
        vertexIndices.push_back(atoi(match.str(1).c_str()));
        uvIndices.push_back(atoi(match.str(2).c_str()));
        normalIndices.push_back(atoi(match.str(3).c_str()));

        vertexIndices.push_back(atoi(match.str(4).c_str()));
        uvIndices.push_back(atoi(match.str(5).c_str()));
        normalIndices.push_back(atoi(match.str(6).c_str()));

        vertexIndices.push_back(atoi(match.str(7).c_str()));
        uvIndices.push_back(atoi(match.str(8).c_str()));
        normalIndices.push_back(atoi(match.str(9).c_str()));

        start += match.position(0) + match.length(0);
        foundCount++;
    }
    return foundCount;
}

}   // namespace mc
