#ifndef UINTA_CAMERA_CONTROLLER_HPP
#define UINTA_CAMERA_CONTROLLER_HPP

#include <camera.hpp>
#include <math.hpp>

#include <glm/mat4x4.hpp>

struct camera_controller {
    smooth_float target_x = smooth_float(10.0, 0.0);
    smooth_float target_y = smooth_float(10.0, 0.0);
    smooth_float target_z = smooth_float(10.0, 1.0);
    smooth_float target_dst;
    smooth_float target_pitch;
    smooth_float target_yaw;
    smooth_float target_roll;

    camera_controller() {
        // target_z.force(1.0);
    }

    void view_matrix(glm::mat4* const);

    void tick(const float dt) {
        target_x.update(dt);
        target_y.update(dt);
        target_z.update(dt);
        target_dst.update(dt);
        target_yaw.update(dt);
        target_roll.update(dt);
        target_pitch.update(dt);
    }

};

#endif // UINTA_CAMERA_CONTROLLER_HPP
