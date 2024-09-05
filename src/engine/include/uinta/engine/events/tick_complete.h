#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_TICK_COMPLETE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_TICK_COMPLETE_H_

#include "uinta/engine/engine_state.h"
#include "uinta/types.h"

namespace uinta {

class EngineState;

struct TickComplete {
  const EngineState* state;
  time_t duration;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_EVENTS_TICK_COMPLETE_H_
