set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

FUNCTION(CHECK_OQGRAPH)
MESSAGE(STATUS "Configuring OQGraph")
FIND_PACKAGE(Boost 1.40.0)
IF(NOT Boost_FOUND)
  MESSAGE(STATUS "Boost not found. OQGraph will not be compiled")
  RETURN()
ENDIF()
INCLUDE_DIRECTORIES(BEFORE ${Boost_INCLUDE_DIRS})

FIND_PACKAGE(Judy)
IF(NOT Judy_FOUND)
  MESSAGE(STATUS "Judy not found. OQGraph will not be compiled")
  RETURN()
ENDIF()
INCLUDE_DIRECTORIES(${Judy_INCLUDE_DIR})

IF(MSVC)
#  # lp:756966 OQGRAPH on Win64 does not compile
#  IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
#    SET(BOOST_OK 0)
#  ELSE()
    SET(BOOST_OK 1)
#  ENDIF()
ELSE()
# See if that works. On old gcc it'll fail because of -fno-rtti
SET(CMAKE_REQUIRED_INCLUDES "${Boost_INCLUDE_DIRS}")
CHECK_CXX_SOURCE_COMPILES(
"
#define BOOST_NO_RTTI 1
#define BOOST_NO_TYPEID 1
#include <boost/config.hpp>
#include <boost/property_map/property_map.hpp>
int main() { return 0; }
" BOOST_OK)
ENDIF()

IF(BOOST_OK)
  ADD_DEFINITIONS(-DHAVE_OQGRAPH)
  IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" PARENT_SCOPE)
    # Fix problem with judy not finding inttypes.h on Windows:
    ADD_DEFINITIONS(-DJU_WIN)
  ELSE(MSVC)
    # Fix lp bug 1221555 with -fpermissive, so that errors in gcc 4.7 become warnings for the time being
    STRING(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    STRING(REPLACE "-fno-implicit-templates" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -fno-strict-aliasing -fpermissive" PARENT_SCOPE)
  ENDIF(MSVC)

  ADD_DEFINITIONS(-DBOOST_NO_RTTI=1 -DBOOST_NO_TYPEID=1 -DBOOST_DISABLE_ASSERTS=1)
  MESSAGE(STATUS "OQGraph OK")
  MYSQL_ADD_PLUGIN(oqgraph ha_oqgraph.cc graphcore.cc graphcore-graph.cc
                   oqgraph_shim.cc oqgraph_thunk.cc oqgraph_judy.cc
                   STORAGE_ENGINE
                   MODULE_ONLY
                   RECOMPILE_FOR_EMBEDDED
                   COMPONENT oqgraph-engine
                   LINK_LIBRARIES ${Judy_LIBRARIES})
ELSE(BOOST_OK)
  MESSAGE(STATUS "Requisites for OQGraph not met. OQGraph will not be compiled")
ENDIF(BOOST_OK)
ENDFUNCTION()

CHECK_OQGRAPH()
