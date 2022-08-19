function (app name)
  message(STATUS "Adding ${CMAKE_CURRENT_LIST_DIR}/${name}")
  add_executable(${name} ./${name}/main.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/utils/debug_controller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/glfw_input.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/glfw_runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/imgui_util.cpp
    )

  if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${name}/res)
    message(STATUS "Adding ${CMAKE_CURRENT_LIST_DIR}/${name}/res to \$\{UINTA_RES_PATHS\}")
    set(UINTA_RES_PATHS ${UINTA_RES_PATHS} ${CMAKE_CURRENT_LIST_DIR}/${name}/res PARENT_SCOPE)
  endif()

  target_link_libraries(${name} PRIVATE uinta)
  target_include_directories(${name} PRIVATE ${GL_INCLUDES})
endfunction()

# app(camera3d)
# app(font)
# app(model)
# app(quadtree)
# app(ray_picking)
app(post_processing)

set(UINTA_RES_PATHS ${UINTA_RES_PATHS} PARENT_SCOPE)
