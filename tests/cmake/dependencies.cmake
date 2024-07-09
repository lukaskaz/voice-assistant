cmake_minimum_required(VERSION 3.10)

find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem)
# find_package(nlohmann_json 3.8.1 REQUIRED)
# find_package(CURL REQUIRED)

include(ExternalProject)

set(source_dir "${CMAKE_CURRENT_BINARY_DIR}/googletest-src")
set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/googletest-build")
set(install_dir "${CMAKE_CURRENT_BINARY_DIR}")

EXTERNALPROJECT_ADD(
  googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           v1.14.0
  PREFIX            ${install_dir}
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CMAKE_ARGS        -D BUILD_SHARED_LIBS=ON
  INSTALL_COMMAND   ""
  UPDATE_COMMAND    ""
)

include_directories(${source_dir}/googletest/include)
include_directories(${source_dir}/googlemock/include)
link_directories(${build_dir}/lib)


set(source_dir "${CMAKE_CURRENT_BINARY_DIR}/libtts-src")
set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/libtts-build")

EXTERNALPROJECT_ADD(
  libtts
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-tts.git
  GIT_TAG           main
  PATCH_COMMAND     ""
  PREFIX            libtts-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/inc)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_CURRENT_BINARY_DIR}/libstt-src")
set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/libstt-build")

EXTERNALPROJECT_ADD(
  libstt
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-stt.git
  GIT_TAG           main
  PATCH_COMMAND     ""
  PREFIX            libstt-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/inc)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_CURRENT_BINARY_DIR}/libshellcmd-src")
set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/libshellcmd-build")

EXTERNALPROJECT_ADD(
  libshellcmd
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-shellcmd.git
  GIT_TAG           main
  PATCH_COMMAND     ""
  PREFIX            libshellcmd-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/inc)
link_directories(${build_dir}/build)

set(source_dir "${CMAKE_CURRENT_BINARY_DIR}/libgptchat-src")
set(build_dir "${CMAKE_CURRENT_BINARY_DIR}/libgptchat-build")

EXTERNALPROJECT_ADD(
  libgptchat
  GIT_REPOSITORY    https://github.com/lukaskaz/lib-gptchat.git
  GIT_TAG           main
  PATCH_COMMAND     ""
  PREFIX            libgptchat-workspace
  SOURCE_DIR        ${source_dir}
  BINARY_DIR        ${build_dir}
  CONFIGURE_COMMAND mkdir /${build_dir}/build &> /dev/null
  BUILD_COMMAND     cd ${build_dir}/build && cmake -D BUILD_SHARED_LIBS=ON
                    ${source_dir} && make -j 4
  UPDATE_COMMAND    ""
  INSTALL_COMMAND   "" 
  TEST_COMMAND      ""
)

include_directories(${source_dir}/inc)
link_directories(${build_dir}/build)
