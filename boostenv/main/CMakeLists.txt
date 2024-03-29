# Boost library

find_package(Boost COMPONENTS system thread filesystem chrono REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

if(MINGW)
  # Work around a bug in MinGW
  string(REGEX REPLACE "(^| )-std=c\\+\\+0x($| )" " -std=gnu++0x "
         CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CXX_STD_OPT -std=gnu++0x)
else()
  set(CXX_STD_OPT -std=c++0x)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  set(MOZART_GENERATOR_FLAGS "-I/usr/lib/c++/v1")
endif()

# Mozart VM library

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../vm/main"
    "${CMAKE_CURRENT_BINARY_DIR}/../../vm/main")

link_directories("${CMAKE_CURRENT_BINARY_DIR}/../../vm/main")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Boost data types and interfaces

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boostenv.gen
  COMMAND ${LLVM_BUILD_DIR}/bin/clang++ "${CXX_STD_OPT}" -femit-ast
    -Wno-invalid-noreturn -Wno-return-type
    -o ${CMAKE_CURRENT_BINARY_DIR}/boostenv.ast
    -I ${CMAKE_CURRENT_SOURCE_DIR}/../../vm/main
    -DMOZART_GENERATOR
    ${MOZART_GENERATOR_FLAGS}
    ${CMAKE_CURRENT_SOURCE_DIR}/boostenv.hh
  COMMAND generator intfimpl
    ${CMAKE_CURRENT_BINARY_DIR}/boostenv.ast
    ${CMAKE_CURRENT_BINARY_DIR}/
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/boostenv.gen
  DEPENDS generator gensources
  IMPLICIT_DEPENDS CXX "${CMAKE_CURRENT_SOURCE_DIR}/boostenv.hh"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT Processing data types and interfaces in boostenv.hh
  VERBATIM)

add_custom_target(genboostsources
  DEPENDS boostenv.gen
  VERBATIM)

# Boost builtins

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boostenvmodules.genbi
  COMMAND ${LLVM_BUILD_DIR}/bin/clang++ "${CXX_STD_OPT}" -femit-ast
    -Wno-invalid-noreturn -Wno-return-type
    -o ${CMAKE_CURRENT_BINARY_DIR}/boostenvmodules.astbi
    -I ${CMAKE_CURRENT_BINARY_DIR}
    -I ${CMAKE_CURRENT_SOURCE_DIR}/../../vm/main
    -I ${CMAKE_CURRENT_BINARY_DIR}/../../vm/main
    -DMOZART_BUILTIN_GENERATOR
    ${MOZART_GENERATOR_FLAGS}
    ${CMAKE_CURRENT_SOURCE_DIR}/boostenvmodules.hh
  COMMAND generator builtins
    ${CMAKE_CURRENT_BINARY_DIR}/boostenvmodules.astbi
    ${CMAKE_CURRENT_BINARY_DIR}/
    boostenvbuiltins
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/boostenv.gen # hack for dependency management
  COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/boostenvmodules.genbi
  DEPENDS generator gensources genboostsources
  IMPLICIT_DEPENDS CXX "${CMAKE_CURRENT_SOURCE_DIR}/boostenvmodules.hh"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT Processing builtins in boostenvmodules.hh
  VERBATIM)

add_custom_target(genboostbuiltins
  DEPENDS boostenvmodules.genbi
  VERBATIM)

# Boost environment library

add_library(mozartvmboost boostenv.cc boostenvmodules.cc)
add_dependencies(mozartvmboost genboostsources genboostbuiltins)
