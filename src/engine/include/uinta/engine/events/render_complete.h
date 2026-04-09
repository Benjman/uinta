#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_RENDER_COMPLETE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_RENDER_COMPLETE_H_

#include "uinta/types.h"

namespace uinta {

class EngineState;

struct RenderComplete {
  const EngineState* state;
  time_t duration;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_RENDER_COMPLETE_H_
