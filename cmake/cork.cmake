# FIXME: This section was created during the prototyping phase and assumes that
#        cork is already installed in the user's environment. Similar to how we
#        handle other missing dependencies, this should be modified to
#        automatically clone the cork repository and integrate it into the
#        project if the library is not found. Additionally, update the path
#        handling to support non-Unix systems as well.

find_library(CORK_LIBRARY NAMES cork PATHS /usr/lib)
find_path(CORK_INCLUDE_DIR cork.h PATHS /usr/include)

add_library(cork STATIC IMPORTED)
set_target_properties(cork PROPERTIES IMPORTED_LOCATION ${CORK_LIBRARY})

list(APPEND UINTA_INCLUDES "${CORK_INCLUDE_DIR}")
list(APPEND UINTA_LIBS ${CORK_LIBRARY})

message(STATUS "Found submodule: Cork")
