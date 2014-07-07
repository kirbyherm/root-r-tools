############################################################################
# CMakeLists.txt file for building ROOT R package
############################################################################
#Autor: Omar Andres Zapata Mesa 31/05/2013

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/r")

set(libname RInterface)

FIND_PACKAGE(R REQUIRED)
FIND_PACKAGE(Rcpp REQUIRED)
FIND_PACKAGE(RInside REQUIRED)

ROOT_USE_PACKAGE(core)
ROOT_USE_PACKAGE(core/rint)
ROOT_USE_PACKAGE(math/matrix)
ROOT_USE_PACKAGE(io/io)
ROOT_USE_PACKAGE(graf2d/gpad)
ROOT_USE_PACKAGE(hist/hist)

if(${R_FOUND} AND ${RCPP_FOUND} AND ${RINSIDE_FOUND})

FILE(WRITE ${CMAKE_SOURCE_DIR}/r/inc/compilerdata.h
     "#define RINCLUDEPATH \" ${R_INCLUDE_DIRS} ${RCPP_INCLUDE_DIRS} ${RINSIDE_INCLUDE_DIRS} \"\n"
     "#define RLINKEDLIBS  \" ${R_LIBRARIES} ${RCPP_LIBRARIES} ${RINSIDE_LIBRARIES} \"\n")

add_definitions(-DUSE_ROOT_ERROR )


STRING(REGEX REPLACE "-I" "" RCPP_INCLUDE_DIR ${RCPP_INCLUDE_DIR})
STRING(REGEX REPLACE "-I" "" RINSIDE_INCLUDE_DIR  ${RINSIDE_INCLUDE_DIR})
STRING(REGEX REPLACE "-I" "" R_INCLUDE_DIR  ${R_INCLUDE_DIR})


include_directories(${CMAKE_SOURCE_DIR}/r/inc ${RCPP_INCLUDE_DIR} ${RINSIDE_INCLUDE_DIR} ${R_INCLUDE_DIR})


set(R_HEADERS ${CMAKE_SOURCE_DIR}/r/inc/TRInterface.h 
              ${CMAKE_SOURCE_DIR}/r/inc/TRObjectProxy.h 
              ${CMAKE_SOURCE_DIR}/r/inc/TRFunction.h
              ${CMAKE_SOURCE_DIR}/r/inc/TRInternalFunction.h
              ${CMAKE_SOURCE_DIR}/r/inc/TRInternalFunction__ctors.h
              ${CMAKE_SOURCE_DIR}/r/inc/RExports.h
              ${CMAKE_SOURCE_DIR}/r/inc/compilerdata.h
              ${CMAKE_SOURCE_DIR}/r/inc/TRCanvas.h
              ${CMAKE_SOURCE_DIR}/r/inc/TRF1.h              
              ${CMAKE_SOURCE_DIR}/r/inc/TRFile.h              
              ${CMAKE_SOURCE_DIR}/r/inc/TRGraph.h              
              ${CMAKE_SOURCE_DIR}/r/inc/TRRint.h              
              ${CMAKE_SOURCE_DIR}/r/inc/TRSystem.h              
              )
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)


ROOT_GENERATE_DICTIONARY(G__RInterface ${R_HEADERS} MODULE ${libname} LINKDEF LinkDef.h)

# ROOT_GENERATE_ROOTMAP(RInterface LINKDEF LinkDef.h)


ROOT_LINKER_LIBRARY(RInterface *.cxx G__RInterface.cxx LIBRARIES ${R_LIBRARIES} ${RCPP_LIBRARIES} ${RINSIDE_LIBRARIES} DEPENDENCIES Core Matrix Rint RIO Hist Gpad readline)
ROOT_INSTALL_HEADERS()
endif()




