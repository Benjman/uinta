list(APPEND APP_SRCS src/main.cc)
add_executable(app ${APP_SRCS})
list(APPEND APP_PRIVATE_LIBS uinta::engine)
list(APPEND APP_PRIVATE_LIBS uinta::platform)
target_link_libraries(app PRIVATE ${APP_PRIVATE_LIBS} ${UINTA_LIBS})
target_include_directories(app PRIVATE ${UINTA_INCLUDES})
list(APPEND UINTA_RES_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(UINTA_RES_PATHS
    ${UINTA_RES_PATHS}
    PARENT_SCOPE)
