
EXECUTABLE_NAME = test_SMDIPCs

#CPP_FILES = smdipcs.cpp 
CPP_FILES =$(shell find . -maxdepth 1 -iname '*.cpp')

SHARED_LIBRARIES += systemcall
SHARED_LIBRARIES += log4cxx
SHARED_LIBRARIES += common
SHARED_LIBRARIES += cppunit


EXECUTABLE_DIR=$(ROOT_DIR)/bin/

include $(ROOT_DIR)/Rules.make