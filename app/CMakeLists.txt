function (app name)
  add_executable(${name} ${CMAKE_CURRENT_LIST_DIR}/${name}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/uinta/glfw/glfw_input.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/uinta/glfw/glfw_runner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/uinta/glfw/glfw_runner_ui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/include/uinta/glfw/imgui.cpp
    )

  if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${name}/res)
    set(UINTA_RES_PATHS ${UINTA_RES_PATHS} ${CMAKE_CURRENT_LIST_DIR}/${name}/res PARENT_SCOPE)
    message(STATUS "Added '${CMAKE_CURRENT_LIST_DIR}/${name}/res' to UINTA_RES_PATHS")
  endif()

  target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
  target_include_directories(${name} PRIVATE ${GL_INCLUDES})
  target_link_libraries(${name} PRIVATE uinta)

  message(STATUS "Added app '${name}'")
endfunction()

app(demo)
app(post_processing)

set(UINTA_RES_PATHS ${UINTA_RES_PATHS} PARENT_SCOPE)
