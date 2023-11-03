# Install script for directory: C:/Users/hchas/Programming/Repos/ChessEngineTest/src/libcuckoo

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/install/x64-debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/libcuckoo/libcuckoo-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/libcuckoo/libcuckoo-targets.cmake"
         "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/build/x64-debug/libcuckoo/CMakeFiles/Export/75bb23512b5425b712dad2372e5f21bd/libcuckoo-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/libcuckoo/libcuckoo-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/libcuckoo/libcuckoo-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/libcuckoo" TYPE FILE FILES "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/build/x64-debug/libcuckoo/CMakeFiles/Export/75bb23512b5425b712dad2372e5f21bd/libcuckoo-targets.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/libcuckoo" TYPE FILE FILES
    "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/libcuckoo/libcuckoo-config.cmake"
    "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/build/x64-debug/libcuckoo/libcuckoo-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/install/x64-debug/include/libcuckoo/cuckoohash_config.hh;C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/install/x64-debug/include/libcuckoo/cuckoohash_map.hh;C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/install/x64-debug/include/libcuckoo/cuckoohash_util.hh;C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/install/x64-debug/include/libcuckoo/libcuckoo_bucket_container.hh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/out/install/x64-debug/include/libcuckoo" TYPE FILE FILES
    "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/libcuckoo/cuckoohash_config.hh"
    "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/libcuckoo/cuckoohash_map.hh"
    "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/libcuckoo/cuckoohash_util.hh"
    "C:/Users/hchas/Programming/Repos/ChessEngineTest/src/libcuckoo/libcuckoo_bucket_container.hh"
    )
endif()

