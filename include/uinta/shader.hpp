#ifndef UINTA_SHADER_H
#define UINTA_SHADER_H

#include <string>
#include <uinta/fwd.hpp>
#include <vector>

namespace uinta {

uinta_error_code createShaderProgram(u32& ref, const std::vector<std::string>& sources, const std::vector<u32>& stages,
                                     const std::vector<std::string>& uniformNames = std::vector<std::string>(),
                                     const std::vector<u32*>& uniformLocations = std::vector<u32*>());

}  // namespace uinta

#endif  // UINTA_SHADER_H
