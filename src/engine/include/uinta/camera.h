#ifndef SRC_ENGINE_INCLUDE_UINTA_CAMERA_H_
#define SRC_ENGINE_INCLUDE_UINTA_CAMERA_H_

#include "uinta/engine_state.h"
#include "uinta/input.h"
#include "uinta/math.h"
#include "uinta/types.h"

namespace uinta {

class EngineState;

struct CameraConfig {
  f32 anglSpdK = 90;       // Angle delta from keyboard input
  f32 anglSpdM = 20;       // Angle delta from mouse input
  f32 dstMax = 10;         // Maximum value when distance is clamped
  f32 dstMin = 0.2;        // Minimum value when distance is clamped
  f32 dstSpdK = 15;        // Distance delta from keyboard input
  f32 dstSpdM = 30;        // Distance delta from mouse input
  f32 far = 200;           // Far clipping plane
  f32 fov = 45;            // Field of view
  f32 near = 0.01;         // Near clipping plane
  f32 pitchMax = 90;       // Maximum value when pitch is clamped
  f32 pitchMin = 0.5;      // Minimum value when pitch is clamped
  f32 pitchSpdK = 90;      // Pitch delta from keyboard input
  f32 pitchSpdM = 20;      // Pitch delta from mouse input
  f32 spdFactor = 0.3;     // Translation speed scales proportional to distance
  f32 spdFactorMin = .66;  // Minimum speed factor
  f32 trnslSpdK = 10;      // Translation speed for keyboard input
  f32 trnslSpdM = 2.5;     // Translation speed for mouse input
  f32 orthoSize = 60;      // Size for the orthographic matrix boundaries
  f32 vertOff = 0;         // Vertical offset

  InputKey anglNegK = KEY_R;   // Keyboard input for negative angle delta
  InputKey anglPosK = KEY_W;   // Keyboard input for positive angle delta
  InputKey dstDecK = KEY_C;    // Keyboard input for decreasing distance
  InputKey dstIncK = KEY_V;    // Keyboard input for increasing distance
  InputKey pitchNegK = KEY_A;  // Keyboard input for negative pitch delta
  InputKey pitchPosK = KEY_Q;  // Keyboard input for positive pitch delta
  InputKey backwardK = KEY_D;  // Keyboard input for translation backward
  InputKey forwardK = KEY_E;   // Keyboard input for translation forward
  InputKey leftK = KEY_S;      // Keyboard input for translation left
  InputKey rightK = KEY_F;     // Keyboard input for translation right

  MouseButton angleM = MOUSE_BUTTON_RIGHT;  // Mouse input for angle
  MouseButton pitchM = MOUSE_BUTTON_RIGHT;  // Mouse input for pitch
  MouseButton trnslM = MOUSE_BUTTON_LEFT;   // Mouse button for translate
};

class Camera {
  using CameraFlags = bitflag8;

  static constexpr bitflag8 DistLimitMask = 1 << 0;
  static constexpr bitflag8 PitchLimitMask = 1 << 1;
  static constexpr bitflag8 LoggingMask = 1 << 2;

 public:
  explicit Camera(CameraConfig config = {}) noexcept : config_(config) {}

  Camera(const Camera&) noexcept = delete;
  Camera& operator=(const Camera&) noexcept = delete;

  Camera(Camera&&) noexcept;
  Camera& operator=(Camera&&) noexcept;

  const SmoothFloat& angle() const noexcept { return angle_; }

  void angle(f32 angle) noexcept { angle_ = angle; }

  const CameraConfig& config() const noexcept { return config_; }

  const SmoothFloat& dist() const noexcept { return dist_; }

  void dist(f32 dist) noexcept { dist_ = dist; }

  glm::mat4 perspectiveMatrix(f32 aspectRatio) const noexcept;

  glm::mat4 orthographicMatrix() const noexcept;

  const SmoothFloat& pitch() const noexcept { return pitch_; }

  void pitch(f32 pitch) noexcept { pitch_ = pitch; }

  const glm::vec3& position() const noexcept { return position_; }

  const SmoothVec3& target() const noexcept { return target_; }

  void target(const SmoothVec3& target) noexcept { target_ = target; }

  void target(const glm::vec3& target) noexcept { target_ = target; }

  void update(const EngineState&, const Input&) noexcept;

  void force() noexcept {
    target_.force();
    angle_.force();
    dist_.force();
    pitch_.force();
  }

  glm::mat4 viewMatrix() const noexcept;

  void limitDist(bool v) noexcept {
    flags_ &= ~DistLimitMask;
    if (v) flags_ |= +DistLimitMask;
  }

  void limitPitch(bool v) noexcept {
    flags_ &= ~PitchLimitMask;
    if (v) flags_ |= +PitchLimitMask;
  }

 protected:
  CameraConfig& config() noexcept { return config_; }

  void config(const CameraConfig config) noexcept { config_ = config; }

 private:
  CameraConfig config_ = {};
  glm::vec3 position_ = {};
  SmoothVec3 target_ = SmoothVec3(8);
  SmoothFloat angle_ = SmoothFloat(8);
  SmoothFloat dist_ = SmoothFloat(8);
  SmoothFloat pitch_ = SmoothFloat(8);
  CameraFlags flags_ = DistLimitMask | PitchLimitMask;

  void processInput(const EngineState&, const Input&) noexcept;
  void updatePosition(const EngineState&, const Input&) noexcept;
  void updateAngle(f32, const Input&) noexcept;
  void updateDist(f32, const Input&) noexcept;
  void updatePitch(f32, const Input&) noexcept;
  void updateTranslation(f32, const Input&) noexcept;

  f32 translationFactor() const noexcept;

  class CameraLogger {
   public:
    CameraLogger() noexcept : interval(1) {}

    CameraLogger(const CameraLogger& other) noexcept
        : interval(other.interval), nextBroadcast(other.nextBroadcast) {}

    CameraLogger& operator=(const CameraLogger& other) noexcept {
      if (this != &other) {
        interval = other.interval;
        nextBroadcast = other.nextBroadcast;
      }
      return *this;
    }

    bool isLogging(const CameraFlags flags) const noexcept {
      return flags & LoggingMask;
    }

    void update(Camera* camera, const EngineState& state,
                const Input& input) noexcept {
      if (input.ctrlDown() && input.isKeyPressed(KEY_L)) {
        if (isLogging(camera->flags_))
          camera->flags_ &= ~LoggingMask;
        else
          camera->flags_ |= LoggingMask;
      }

      if (!isLogging(camera->flags_)) return;

      if ((nextBroadcast -= state.delta()) < 0) {
        log(camera);
        nextBroadcast = interval;
      }
    }

   private:
    f32 interval;
    f32 nextBroadcast = 0;

    void log(const Camera*) const noexcept;
  } logger_;
};

}  // namespace uinta

#endif  // SRC_ENGINE_INCLUDE_UINTA_CAMERA_H_
