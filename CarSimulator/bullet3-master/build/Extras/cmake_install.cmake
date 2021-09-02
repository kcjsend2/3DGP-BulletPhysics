# Install script for directory: C:/Users/kcjse/Desktop/bullet3-master/Extras

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/BULLET_PHYSICS")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/InverseDynamics/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/BulletRoboticsGUI/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/BulletRobotics/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/obj2sdf/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/Serialize/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/ConvexDecomposition/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/HACD/cmake_install.cmake")
  include("C:/Users/kcjse/Desktop/bullet3-master/build/Extras/GIMPACTUtils/cmake_install.cmake")

endif()

