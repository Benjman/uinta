#include <uinta/glfw/imgui.h>
#include <uinta/types.h>

#include <chrono>

namespace uinta {

class TimingController {
 public:
  void render(const char* const label) {
    f32 avg = 0;
    if (count) {
      for (u32 i = count; i; --i) avg += buffer[(tail - i + count) % count];
      avg /= count;
    }
    ImGui::PlotLines(label, buffer, count, head - 1, std::to_string(avg).c_str());
  }

  void start() {
    startTime = timer::now();
  }

  void stop() {
    buffer[tail] = std::chrono::duration_cast<std::chrono::microseconds>(timer::now() - startTime).count();
    buffer[tail] /= 1000.0;
    count = std::min(count + 1, size);
    tail = (tail + 1) % size;
    if (count == size) head = (head + 1) % size;
  }

 private:
  using timer = std::chrono::high_resolution_clock;

  static constexpr u32 size = 100;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  u32 head = 0;
  u32 tail = 0;
  u32 count = 0;
  f32 buffer[size];
};

}  // namespace uinta
