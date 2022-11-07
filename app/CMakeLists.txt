function (app name)
  add_executable(${name} ./${name}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils/utils.cpp
    )

  if (IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${name}/res)
    set(UINTA_RES_PATHS ${UINTA_RES_PATHS} ${CMAKE_CURRENT_LIST_DIR}/${name}/res PARENT_SCOPE)
    message(STATUS "Added '${CMAKE_CURRENT_LIST_DIR}/${name}/res' to UINTA_RES_PATHS")
  endif()

  target_include_directories(${name} PRIVATE ${GL_INCLUDES})
  target_link_libraries(${name} PRIVATE uinta)

  message(STATUS "Added app '${name}'")
endfunction()

set(UINTA_RES_PATHS ${UINTA_RES_PATHS} PARENT_SCOPE)
