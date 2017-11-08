# FindPhpcpp
# --------
#
# Find ArangoDB Phpcpp
#
# ::
#
#   PHPCPP_INCLUDE_DIRS   - where to find phpcpp/phpcpp.h, etc.
#   PHPCPP_LIBRARIES      - List of libraries when using phpcpp.
#   PHPCPP_FOUND          - True if phpcpp found.
#   PHPCPP_VERSION_STRING - the version of phpcpp found (since CMake 2.8.8)

# Look for the header file.
find_path(PHPCPP_INCLUDE_DIR NAMES phpcpp.h)
mark_as_advanced(PHPCPP_INCLUDE_DIR)

# Look for the library (sorted from most current/relevant entry to least).
find_library(PHPCPP_LIBRARY NAMES
    libphpcpp.so
)
mark_as_advanced(PHPCPP_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set PHPCPP_FOUND to TRUE if
# all listed variables are TRUE
include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PHPCPP
                                  REQUIRED_VARS PHPCPP_LIBRARY PHPCPP_INCLUDE_DIR
                                  VERSION_VAR PHPCPP_VERSION_STRING)

if(PHPCPP_FOUND)
  set(PHPCPP_LIBRARIES ${PHPCPP_LIBRARY})
  set(PHPCPP_INCLUDE_DIRS ${PHPCPP_INCLUDE_DIR})
endif()
