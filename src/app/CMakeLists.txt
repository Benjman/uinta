list(APPEND APP_SRCS src/main.cc)
list(APPEND APP_SRCS src/scenes/city_game/city_game_scene.cc)
list(APPEND APP_SRCS src/scenes/city_game/intersection_init.cc)
list(APPEND APP_SRCS src/scenes/city_game/intersection_renderer.cc)
list(APPEND APP_SRCS src/scenes/city_game/movement_system.cc)
list(APPEND APP_SRCS src/scenes/city_game/pawn_renderer.cc)
list(APPEND APP_SRCS src/scenes/city_game/pawn_system.cc)

add_executable(app ${APP_SRCS})
list(APPEND APP_PRIVATE_LIBS uinta::engine)
list(APPEND APP_PRIVATE_LIBS uinta::platform)
target_link_libraries(app PRIVATE ${APP_PRIVATE_LIBS} ${UINTA_LIBS})
target_include_directories(app PRIVATE include ${UINTA_INCLUDES})
list(APPEND UINTA_RES_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(UINTA_RES_PATHS
    ${UINTA_RES_PATHS}
    PARENT_SCOPE)
