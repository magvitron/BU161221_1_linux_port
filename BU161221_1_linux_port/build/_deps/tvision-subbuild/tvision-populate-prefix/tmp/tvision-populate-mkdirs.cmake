# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-src"
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-build"
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix"
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix/tmp"
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix/src/tvision-populate-stamp"
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix/src"
  "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix/src/tvision-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix/src/tvision-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/shreyas_home/BU_16_NEW/BU161221_1_linux_port/build/_deps/tvision-subbuild/tvision-populate-prefix/src/tvision-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
