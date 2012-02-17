#    This file is part of REQuest project
#    Copyright (C) 2008-2009 Filip Volejnik
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    $Id: CMakeLists.txt 723 2008-04-16 17:03:52Z volca $

cmake_minimum_required(VERSION 2.4)
project (ReQuest)

SET(REQUEST_VER_MAJOR "0")
SET(REQUEST_VER_MINOR "0")
SET(REQUEST_VER_PATCH "1")

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# Warning level high (Lowered by one to supress the payload of ogre related warnings in VC)
SET(CMAKE_CXX_WARNING_LEVEL 4)

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_C_FLAGS_DEBUG "-g -O3")
    SET(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
    SET(CMAKE_CXX_FLAGS_DISTRIBUTION "-g -O3")
    SET(CMAKE_C_FLAGS_DISTRIBUTION "-g -O3")
    SET(CMAKE_CXX_FLAGS "-g")
    ADD_DEFINITIONS(-Wall -fno-strict-aliasing -fvisibility=default)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# Various modules used for library path detections.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Solve the dependencies
SET(OIS_DIR ${CMAKE_MODULE_PATH})
SET(OGRE_DIR ${CMAKE_MODULE_PATH})

FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(OGRE REQUIRED)

# Only visual check enabling verbose output, and package found info
IF(OIS_FOUND)
     MESSAGE(STATUS "OIS Found OK")
     MESSAGE(STATUS " - OIS includes ${OIS_INCLUDE_DIR}")
     MESSAGE(STATUS " - OIS libs ${OIS_LIBRARIES}")
ELSE(OIS_FOUND)
     MESSAGE(FATAL_ERROR "OIS not found")
ENDIF(OIS_FOUND)

IF(OGRE_FOUND)
    MESSAGE(STATUS "OGRE Found OK")
    MESSAGE(STATUS " - Ogre includes ${OGRE_INCLUDE_DIR}")
    MESSAGE(STATUS " - Ogre libs ${OGRE_LIBRARIES}")
ELSE(OGRE_FOUND)
    MESSAGE(FATAL_ERROR "OGRE not found")
ENDIF(OGRE_FOUND)

# Some configuration checks to allow platform independence
CONFIGURE_FILE(config.h.in ${CMAKE_BINARY_DIR}/config.h )

# Add the subdirectories which contain aditional CMakeLists.txt files
add_subdirectory (src)

MESSAGE(STATUS "Your configuration seems to be OK!")
