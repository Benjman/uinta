#ifndef UINTA_RUNNER_FWD_HPP
#define UINTA_RUNNER_FWD_HPP

#include <uinta/flags.hpp>

namespace uinta {

struct Display;
struct RunnerState;
class Runner;

inline bool isCameraEnabled(flags_t flags);
inline bool isGridEnabled(flags_t flags);

}  // namespace uinta

#endif  // UINTA_RUNNER_FWD_HPP
