set(POLY2TRI_DIR "${UINTA_LIB_DIR}/poly2tri")
if(NOT EXISTS "${POLY2TRI_DIR}")
  message(FATAL_ERROR
    "poly2tri library not found. Likely missing git submodules.")
endif()
add_subdirectory(${POLY2TRI_DIR})
list(APPEND UINTA_LIBS poly2tri)
