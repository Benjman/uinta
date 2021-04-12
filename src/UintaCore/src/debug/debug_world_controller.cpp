#include <uinta/debug/debug_controller.h>
#include <uinta/debug/debug_world_controller.h>

using namespace uinta;

DebugWorldController::DebugWorldController(DebugController *parent)
		: BufferController(parent, KILOBYTES(5), KILOBYTES(2)) {
}
