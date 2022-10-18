function (app name)
  add_executable(${name} ${CMAKE_CURRENT_LIST_DIR}/${name}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/glfw_input.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/glfw_runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/imgui_util.cpp
    )

  if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${name}/res)
    set(UINTA_RES_PATHS ${UINTA_RES_PATHS} ${CMAKE_CURRENT_LIST_DIR}/${name}/res PARENT_SCOPE)
    message(STATUS "Added '${CMAKE_CURRENT_LIST_DIR}/${name}/res' to UINTA_RES_PATHS")
  endif()

  target_include_directories(${name} PRIVATE ${GL_INCLUDES})
  target_link_libraries(${name} PRIVATE uinta)

  message(STATUS "Added app '${name}'")
endfunction()

app(camera)
# app(model)
# app(ray_picking)
# app(post_processing)

set(UINTA_RES_PATHS ${UINTA_RES_PATHS} PARENT_SCOPE)
