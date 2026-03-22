find_package(assimp REQUIRED)
message(STATUS "Found installed Assimp: ${assimp_VERSION}")

list(APPEND UINTA_LIBS assimp)