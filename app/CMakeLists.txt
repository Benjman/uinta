function (app name)
    message(STATUS "Adding ${CMAKE_CURRENT_LIST_DIR}/${name}")
    add_executable(${name} ./${name}/main.cpp
      ${CMAKE_CURRENT_LIST_DIR}/utils/debug_controller.cpp
      ${CMAKE_CURRENT_LIST_DIR}/utils/glfw_input.cpp
      ${CMAKE_CURRENT_LIST_DIR}/utils/glfw_runner.cpp
      ${CMAKE_CURRENT_LIST_DIR}/utils/imgui_util.cpp
      )
    target_link_libraries(${name} PRIVATE uinta)
    target_include_directories(${name} PRIVATE ${GL_INCLUDES})
endfunction()

app(camera3d)
app(font)
app(model)
app(quadtree)
app(ray_picking)
app(terrain)
