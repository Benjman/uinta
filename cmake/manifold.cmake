find_package(manifold REQUIRED)
message(STATUS "Found installed manifold: ${manifold_VERSION}")

list(APPEND UINTA_LIBS manifold::manifold)
