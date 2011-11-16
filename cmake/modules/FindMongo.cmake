
# - Find MongoDB
# Find the MongoDB includes and client library
# This module defines
#  MONGO_INCLUDE_DIR, where to find client/dbclient.h
#  MONGO_LIBRARIES, the libraries needed to use MONGO.
#  MONGO_FOUND, If false, do not try to use MongoDB.
#
# Copyright (c) 2011, Philipp Fehre, <philipp.fehre@googlemail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Add the mongodb include paths here

if(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
   set(MONGO_FOUND TRUE)

else(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)

	find_path(MONGO_INCLUDE_DIR client/dbclient.h
	  /usr/include/mongo
    )

  find_library(MONGO_LIBRARIES NAMES mongoclient  libmongoclient 
    PATHS
    /usr/lib
    )

  if(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
    set(MONGO_FOUND TRUE)
    message(STATUS "Found MongoDB: ${MONGO_INCLUDE_DIR}, ${MONGO_LIBRARIES}")
    INCLUDE_DIRECTORIES(${MONGO_INCLUDE_DIR})
  else(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
    set(MONGO_FOUND FALSE)
    message(STATUS "MongoDB not found.")
  endif(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)

  mark_as_advanced(MONGO_INCLUDE_DIR MONGO_LIBRARIES)

endif(MONGO_INCLUDE_DIR AND MONGO_LIBRARIES)
