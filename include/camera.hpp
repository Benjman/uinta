#ifndef UINTA_CAMERA2D_H
#define UINTA_CAMERA2D_H

#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <math.hpp>
#include <runner.hpp>

/** world coordinates follow the "right-handed rule" **/
const inline glm::vec3 WORLD_UP      = glm::vec3(0.0, 1.0,  0.0);
const inline glm::vec3 WORLD_RIGHT   = glm::vec3(1.0, 0.0,  0.0);
const inline glm::vec3 WORLD_FORWARD = glm::vec3(0.0, 0.0, -1.0);

struct CameraConfig {
    float fov = 40.0;
    float aspectRatio = 1.0;
    float nearPlane = 0.1;
    float farPlane = 1000.0;
    float yOffset = 150.0;
    float minZoom = 1.0;
    float maxZoom = 55.0;
    float minPitch = 26.0;
    float maxPitch = 55.0;
    float moveAbility = 27.0;
    float rotateAgility = 10.0;
    float zoomAgility = 8.0;
    float startZoom = 55.0;
    float startAngle = -30.0;
    float dragSensitivity = 0.1;
    float keyMoveSpeed = 1.0;
    float zoomSensitivity = 5.0;
    float yawSensitivity = 1.0;
    float cinRotateSpeed = 0.06;

    glm::vec3 resetTarget = glm::vec3(15.0, yOffset, 15.0);
    glm::vec3 startTarget = glm::vec3(resetTarget);

    CameraConfig(const RunnerState& runner) {
        aspectRatio = runner.display.aspectRatio;
    }

    CameraConfig(const CameraConfig& other) {
        *this = other;
    }

    CameraConfig& operator=(const CameraConfig& other) {
        fov = other.fov;
        aspectRatio = other.aspectRatio;
        nearPlane = other.nearPlane;
        farPlane = other.farPlane;
        yOffset = other.yOffset;
        minZoom = other.minZoom;
        maxZoom = other.maxZoom;
        minPitch = other.minPitch;
        maxPitch = other.maxPitch;
        moveAbility = other.moveAbility;
        rotateAgility = other.rotateAgility;
        zoomAgility = other.zoomAgility;
        startZoom = other.startZoom;
        startAngle = other.startAngle;
        dragSensitivity = other.dragSensitivity;
        keyMoveSpeed = other.keyMoveSpeed;
        zoomSensitivity = other.zoomSensitivity;
        yawSensitivity = other.yawSensitivity;
        cinRotateSpeed = other.cinRotateSpeed;
        resetTarget = other.resetTarget;
        startTarget = other.startTarget;
        return *this;
    }

};

struct CameraControls {
    float zoom(const RunnerState& state, const CameraConfig& configs) {
        return state.input.scrolldy * configs.zoomSensitivity;
    }

    float goRight(const RunnerState& state, const CameraConfig&) {
        return state.input.isKeyDown(KEY_D) || state.input.isKeyDown(KEY_RIGHT);
    }

    float goLeft(const RunnerState& state, const CameraConfig&) {
        return state.input.isKeyDown(KEY_A) || state.input.isKeyDown(KEY_LEFT);
    }

    float goForwards(const RunnerState& state, const CameraConfig&) {
        return state.input.isKeyDown(KEY_W) || state.input.isKeyDown(KEY_UP);
    }

    float goBackwards(const RunnerState& state, const CameraConfig&) {
        return state.input.isKeyDown(KEY_S) || state.input.isKeyDown(KEY_DOWN);
    }

    float rotation(const RunnerState& state, const CameraConfig& configs) {
        if (state.input.isKeyDown(KEY_C))
            return configs.cinRotateSpeed;
        if (state.input.isKeyDown(KEY_V))
            return -configs.cinRotateSpeed;
        if (!state.input.isMouseButtonDown(MOUSE_BUTTON_MIDDLE))
            return 0.0;
        return state.input.cursordx * configs.yawSensitivity;
    }

    bool isDragging(const RunnerState& state, const CameraConfig&) {
        return state.input.isMouseButtonDown(MOUSE_BUTTON_RIGHT);
    }

    bool reset(const RunnerState& state, const CameraConfig&) {
        return state.input.isKeyPressed(KEY_R);
    }

    glm::vec2 dragFactor(const RunnerState& state, const CameraConfig& configs) {
        return glm::vec2(state.input.cursordx, state.input.cursordy) * glm::vec2(configs.dragSensitivity);
    }

    glm::vec2 movements(const RunnerState& state, const CameraConfig& configs) {
        glm::vec2 result(0);
        float speed = configs.keyMoveSpeed * state.delta;
        if (goForwards(state, configs))
            result.y = speed;
        else if (goBackwards(state, configs))
            result.y = -speed;
        if (goRight(state, configs))
            result.x = speed;
        else if (goLeft(state, configs))
            result.x = -speed;
        return result;
    }

    glm::vec2 moveDirection(const RunnerState& state, const CameraConfig& configs) {
        if (isDragging(state, configs))
            return dragFactor(state, configs);
        return movements(state, configs);
    }

};

struct Camera {
    CameraConfig configs;
    CameraControls controls;

    glm::mat4 view = glm::mat4(1.0);
    glm::vec3 position = glm::vec3(0.0);
    float pitch, yaw;

    smooth_vec3 target;
    smooth_float angle;
    smooth_float dist;

    Camera(const CameraConfig& configs) :
        configs(configs),
        target(configs.startTarget),
        angle(configs.startAngle, configs.rotateAgility),
        dist(configs.startZoom, configs.zoomAgility) {
    }

    void tick(const RunnerState& state) {
        // process inputs
        // if (controls.reset(state, configs))
        //     reset();

        // angle
        float angleDelta = controls.rotation(state, configs);
        angle -= angleDelta;
        angle.tick(state.delta);

        // zoom
        float targetZoom = dist.target;
        float zoomLevel = controls.zoom(state, configs);
        targetZoom -= zoomLevel;
        targetZoom = glm::clamp(targetZoom, configs.minZoom, configs.maxZoom);
        dist = targetZoom;
        dist.tick(state.delta);

        // target
        glm::vec2 move = controls.moveDirection(state, configs);
        float speed = dist.current;
        float yaw_rad = glm::radians(yaw);
        float yaw_side_rad = glm::radians(yaw + 90.0);
        float dx = move.y * speed * -glm::sin(yaw_rad);
        float dz = move.y * speed *  glm::cos(yaw_rad);
        float sideDx = move.x * speed *  glm::sin(yaw_side_rad);
        float sideDz = move.x * speed * -glm::cos(yaw_side_rad);
        target += glm::vec3(dx + sideDx, 0, dz + sideDz);
        target.tick(state.delta);

        yaw = std::fmod(360.0 - angle.current, 360.0);
        pitch = map0to1RangeClamped(dist.current, configs.minZoom, configs.maxPitch);
        pitch = dist.current;

        // update position
        float pitch_rad = glm::radians(pitch);
        float angle_rad = glm::radians(angle.current);
        float horizontal = dist.current * glm::cos(pitch_rad);
        float vertical = dist.current * glm::sin(pitch_rad);
        float x = target.x() + horizontal * glm::sin(angle_rad);
        float y = target.y() + vertical;
        float z = target.z() + horizontal * glm::cos(angle_rad);
        position = glm::vec3(x, y, z);
    }

};

#endif // UINTA_CAMERA2D_HglfwSetWindowSizeCallback
