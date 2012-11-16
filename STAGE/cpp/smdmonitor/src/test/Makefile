
EXECUTABLE_NAME = test_SMDLAUNCHER

CPP_FILES = $(shell find . -maxdepth 1 -iname '*.cpp') ../main/launcher/SMDLauncher.cpp

SHARED_LIBRARIES += operationSMD
SHARED_LIBRARIES += curlCall
SHARED_LIBRARIES += SmdExecption
SHARED_LIBRARIES += curl
SHARED_LIBRARIES += json_spirit
SHARED_LIBRARIES += common
SHARED_LIBRARIES += log4cxx
SHARED_LIBRARIES += pthread
SHARED_LIBRARIES += systemcall
SHARED_LIBRARIES += cppunit


EXECUTABLE_DIR=$(ROOT_DIR)/bin/

include $(ROOT_DIR)/Rules.make