#ifndef SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STAGE_H_
#define SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STAGE_H_

#include "uinta/types.h"

namespace uinta {

enum class EngineStage : u8 {
  PreTick,
  Tick,
  PostTick,
  PreRender,
  Render,
  PostRender,
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_ENGINE_ENGINE_STAGE_H_
