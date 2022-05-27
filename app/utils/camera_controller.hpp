#ifndef UINTA_CAMERA_CONTROLLER_HPP
#define UINTA_CAMERA_CONTROLLER_HPP

#include <camera.hpp>
#include <math.hpp>

#include <glm/mat4x4.hpp>

struct debug_camera : public target_cam {
};

struct camera_controller {
    smooth_float target_x = smooth_float(0.0, 10.0);
    smooth_float target_y = smooth_float(0.0, 10.0);
    smooth_float target_z = smooth_float(0.0, 10.0);
    smooth_float target_dst;
    smooth_float target_pitch;
    smooth_float target_yaw;
    smooth_float target_roll;

    void tick(const float dt) {
        target_x.tick(dt);
        target_y.tick(dt);
        target_z.tick(dt);
        target_dst.tick(dt);
        target_yaw.tick(dt);
        target_roll.tick(dt);
        target_pitch.tick(dt);
    }

};

#endif // UINTA_CAMERA_CONTROLLER_HPP
