#ifndef UINTA_FWD_HPP
#define UINTA_FWD_HPP

#include <uinta/utils/flags.h>
#include <uinta/utils/macros.h>
#include <uinta/utils/types.h>

#include <uinta/gl/fwd.hpp>
#include <uinta/input/fwd.hpp>
#include <uinta/math/fwd.hpp>
#include <uinta/runner/fwd.hpp>

namespace uinta {

using file_t = uint;
using file_size_t = uint;

struct MemoryLink;
struct MeshAttrib;
struct MetricsController;
struct Quad;
struct RunningAvg;

class CartesianGrid;
class FileManager;

}  // namespace uinta

#endif  // UINTA_FWD_HPP
