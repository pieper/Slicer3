project(QueryAtlas)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)

 
# --------------------------------------------------------------------------
# Find Slicer3

if(NOT Slicer3_SOURCE_DIR)
  find_package(Slicer3 REQUIRED)
  include(${Slicer3_USE_FILE})
  slicer3_set_default_install_prefix_for_external_projects()
endif(NOT Slicer3_SOURCE_DIR)

# --------------------------------------------------------------------------
# Include dirs
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${Slicer3_Libs_INCLUDE_DIRS}
  ${Slicer3_Base_INCLUDE_DIRS}
  )

# This module use's FreeType, which is not quite exported correctly by an
# installed VTK. It's not obvious either if this should be exposed 
# directly by VTK, but this is an easy fix.

if(VTK_INSTALL_PREFIX)
  include_directories(
    ${VTK_INSTALL_PREFIX}/${VTK_INSTALL_INCLUDE_DIR}/vtkfreetype/include
    )
endif(VTK_INSTALL_PREFIX)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/vtkQueryAtlasConfigure.h.in 
  ${CMAKE_CURRENT_BINARY_DIR}/vtkQueryAtlasConfigure.h
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES 
  ${headers} 
  "${CMAKE_CURRENT_BINARY_DIR}/vtkQueryAtlasConfigure.h"
  DESTINATION ${Slicer3_INSTALL_MODULES_INCLUDE_DIR}/${PROJECT_NAME} COMPONENT Development
  )

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.h")
install(FILES 
  ${headers} 
  DESTINATION ${Slicer3_INSTALL_MODULES_INCLUDE_DIR}/${PROJECT_NAME}/Resources COMPONENT Development
  ) 

# --------------------------------------------------------------------------
# Sources

set(QueryAtlas_SRCS
  vtkQueryAtlasIcons.cxx        
  vtkQueryAtlasLogic.cxx
  vtkQueryAtlasGUI.cxx
  vtkMRMLQueryAtlasNode.cxx
  vtkQueryAtlasCollaboratorIcons.cxx
  vtkQueryAtlasUseSearchTermWidget.cxx  
  vtkQueryAtlasSearchTermWidget.cxx  
  # cards and related classes
  vtkBIRNCard.cxx
  vtkBIRNCardManager.cxx
  vtkCard.cxx
  vtkCardManager.cxx
  vtkEdgeGradient.cxx
  vtkFontParameters.cxx
  vtkFreetypeRasterizer.cxx
  vtkRasterizerCharacter.cxx
  vtkRectangle.cxx
  #vtkSortCommand.cxx
  vtkSorter.cxx
  vtkTextureFont.cxx
  vtkTextureFontManager.cxx
  vtkTextureText.cxx
  vtkTextureTextPolys.cxx
  )

# --------------------------------------------------------------------------
# Wrapping

include("${VTK_CMAKE_DIR}/vtkWrapTcl.cmake")
vtk_wrap_tcl3(QueryAtlas QueryAtlas_TCL_SRCS "${QueryAtlas_SRCS}" "")

# --------------------------------------------------------------------------
# Add Loadable Module support

generatelm(QueryAtlas_SRCS QueryAtlas.xml)

# --------------------------------------------------------------------------
# Build and install the library

set(lib_name QueryAtlas)
add_library(${lib_name}
  ${QueryAtlas_SRCS}
  ${QueryAtlas_TCL_SRCS}
  )
slicer3_set_modules_output_path(${lib_name})

target_link_libraries(${lib_name}
  ${Slicer3_Libs_LIBRARIES}
  ${Slicer3_Base_LIBRARIES}
  ${KWWidgets_LIBRARIES}
  ${ITK_LIBRARIES}
  )

slicer3_install_modules(${lib_name})

# --------------------------------------------------------------------------
# Testing

if(BUILD_TESTING)
  ADD_SUBDIRECTORY(Testing)
endif(BUILD_TESTING)

# --------------------------------------------------------------------------
# Install support files

# Should those paths change, make sure to update 
#   - Tcl/QueryAtlas.tcl
#   - Tcl/QueryAtlasControlledVocabulary.tcl
#   - vtkQueryAtlasGUI.cxx

# Tcl files

# Question: I'm not quite sure if the Tcl scripts here are supposed to be a module loaded at run-time; since there is a pkgindex.tcl I assume it was a Tcl "module", one loaded by ScriptedModule, but said ScriptedModule does choke at run-time trying to call QueryAtlasBuildGUI or QueryAtlasAddGUIObservers (both methods created by appending "BuildGUI" or "AddGUIObservers" to the module name, none of them to be found in the scripts here). Until we find out, let's copy those support files to share/ instead of lib/.  vtkQueryAtlasGUI.cxx seems to add the path to these scripts manually by updating auto_path, so maybe they are just support scripts, but part of a standalone module.

file(GLOB TCLFILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "Tcl/*.tcl")

foreach(file ${TCLFILES})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${file}
    ${CMAKE_BINARY_DIR}/${Slicer3_INSTALL_MODULES_SHARE_DIR}/${PROJECT_NAME}/${file}
    COPYONLY)
endforeach(file)

install(
  FILES ${TCLFILES}
  DESTINATION ${Slicer3_INSTALL_MODULES_SHARE_DIR}/${PROJECT_NAME}/Tcl
  )

# Resources (CSV, txt, xls, .h)

file(GLOB CSVFILES "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.csv")
file(GLOB TXTFILES "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.txt")
file(GLOB XLSFILES "${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.xls")

foreach(file 
    ${CSVFILES} 
    ${TXTFILES}
    ${XLSFILES}
)
  get_filename_component(filename "${file}" NAME)
  configure_file(
    ${file}
    ${CMAKE_BINARY_DIR}/${Slicer3_INSTALL_MODULES_SHARE_DIR}/${PROJECT_NAME}/Resources/${filename}
    COPYONLY)
endforeach(file)

install(FILES 
  ${CSVFILES} 
  ${TXTFILES} 
  ${XLSFILES}
  DESTINATION ${Slicer3_INSTALL_MODULES_SHARE_DIR}/${PROJECT_NAME}/Resources COMPONENT Development
  ) 

# ImageData

file(GLOB IMAGEFILES "${CMAKE_CURRENT_SOURCE_DIR}/ImageData/*.png")

foreach(file ${IMAGEFILES})
  get_filename_component(filename "${file}" NAME)
  configure_file(
    ${file}
    ${CMAKE_BINARY_DIR}/${Slicer3_INSTALL_MODULES_SHARE_DIR}/${PROJECT_NAME}/ImageData/${filename}
    COPYONLY)
endforeach(file)

install(FILES 
  ${IMAGEFILES}
  DESTINATION ${Slicer3_INSTALL_MODULES_SHARE_DIR}/${PROJECT_NAME}/ImageData COMPONENT Development
  ) 

# Ontoviz

set(ONTOVIZFILES
  OntologyViz/bin/birnlexvis-support.jar
  OntologyViz/bin/birnlexvis.jar
  OntologyViz/bin/birnlexvis.tcl
  OntologyViz/bin/birnlexvisUnix.sh
  OntologyViz/bin/birnlexvisWin.sh
  OntologyViz/build.xml
  OntologyViz/data/birnlex-body-v1.json
  OntologyViz/data/neuronames-v1.json
  OntologyViz/extjars/json.jar
  OntologyViz/extjars/jython.jar
  OntologyViz/extjars/prefuse.jar    
  )

foreach(file ${ONTOVIZFILES})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${file}
    ${CMAKE_BINARY_DIR}/${Slicer3_INSTALL_MODULES_LIB_DIR}/${PROJECT_NAME}/${file}
    COPYONLY)
  get_filename_component(path "${file}" PATH)
  install(FILES 
    ${file} 
    DESTINATION ${Slicer3_INSTALL_MODULES_LIB_DIR}/${PROJECT_NAME}/${path} COMPONENT Development
    ) 
endforeach(file)
