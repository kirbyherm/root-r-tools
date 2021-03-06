############################################################################
# CMakeLists.txt file for building ROOT rootfit/histfactory package
# @author Pere Mato, CERN
############################################################################

ROOT_USE_PACKAGE(io/xmlparser)

ROOT_GENERATE_DICTIONARY(G__HistFactory RooStats/HistFactory/*.h LINKDEF LinkDef.h)

ROOT_GENERATE_ROOTMAP(HistFactory LINKDEF LinkDef.h 
                                  DEPENDENCIES RooFit RooFitCore Tree RIO Hist Matrix MathCore Minuit Foam Graf Gpad RooStats XMLParser )
                                               
ROOT_LINKER_LIBRARY(HistFactory  *.cxx G__HistFactory.cxx LIBRARIES Core Cint 
                                DEPENDENCIES RooFit RooFitCore Tree RIO Hist Matrix MathCore Minuit Foam Graf Gpad RooStats XMLParser)
ROOT_EXECUTABLE(hist2workspace MakeModelAndMeasurements.cxx hist2workspace.cxx
			       LIBRARIES HistFactory RooFit RooFitCore Tree RIO Matrix Hist ${ROOT_MATHMORE_LIBRARY} ${GSL_LIBRARIES} MathCore 
                                         Graf Gpad Minuit Foam RooStats XMLParser)

#ROOT_INSTALL_HEADERS()
install(DIRECTORY inc/RooStats/HistFactory/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/RooStats/HistFactory 
                       PATTERN ".svn" EXCLUDE
                       REGEX "LinkDef" EXCLUDE )
set_property(GLOBAL APPEND PROPERTY ROOT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/inc)
