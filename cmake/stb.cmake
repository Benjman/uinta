# Use system-installed stb headers
# Arch Linux installs them to /usr/include/stb/
find_path(STB_INCLUDE_DIR stb/stb_image.h
  PATHS /usr/include /usr/local/include
  REQUIRED)
message(STATUS "Found stb headers: ${STB_INCLUDE_DIR}")
list(APPEND UINTA_INCLUDES "${STB_INCLUDE_DIR}")