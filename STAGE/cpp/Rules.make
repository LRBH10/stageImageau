CC=g++
COMMON_FLAGS=$(OPT_CFLAGS)
CFLAGS=-c -Wextra -Wall -pedantic -Wno-long-long 
CFLAGS+=-Wctor-dtor-privacy -Wnon-virtual-dtor  -Wreorder -Wstrict-null-sentinel
CFLAGS+=-Wsign-promo -ggdb3 -O0 $(COMMON_FLAGS) $(CFLAGS_PLATFORM)
#-D_GLIBCXX_DEBUG=1 -D_GLIBCXX_DEBUG_PEDANTIC=1 

ifdef FORCE_32_BITS
	OPT_CFLAGS += -m32 -mtune=i686
	LIB_SUFFIX=32
	LDFLAGS += -m32
else 
	LIB_SUFFIX=
endif #FORCE_32_BITS

INCLUDE_DIR=$(ROOT_DIR)/include

INCLUDE_DEP=-I$(HOME)/usr/local/include -I/usr/local/include -I. -I/usr/include/
LDFLAGS+=-L$(LIB_DIR) -L$(HOME)/usr/local/lib$(LIB_SUFFIX) -L/usr/local/lib$(LIB_SUFFIX) -L/usr/lib$(LIB_SUFFIX) $(COMMON_FLAGS)  $(LDFLAGS_PLATFORM)
#-Wl,--as-needed 
EXECUTABLE_DIR=$(ROOT_DIR)/bin
LIB_DIR=$(ROOT_DIR)/lib
DUM_INCL=$(ROOT_DIR)/bin/dum_incl

INCLUDE += -I$(INCLUDE_DIR) $(INCLUDE_DEP)

CCDEPEND = $(CC) $(COMMON_FLAGS) -MM $(INCLUDE)

include $(ROOT_DIR)/Rules_opt.make

PLATF = $(shell uname)
ifeq ($(PLATF),CYGWIN_NT-5.1)
  CC=g++
  CFLAGS_PLATFORM=-D_CYGWIN_ -fpermissive
#-Wno-variadic-macros -mtune=native 
  LDFLAGS_PLATFORM=-Wl,--enable-auto-import -L/usr/lib/gcc/i686-pc-cygwin/4.3.4 -lstdc++
else
  ifeq ($(PLATF),Linux)
#    GCC_VERSION = $(shell  g++ --version | egrep -o '[0-9]*\.[0-9]*\.[0-9]*' | head -n1)
    CFLAGS_PLATFORM=-D_LINUX_ -Wno-variadic-macros -Weffc++ -ansi -fPIC
# heavy load of warnings: -Weffc++
    LDFLAGS_PLATFORM=-lrt
# electric fence: -lefence
# DUMA: -lduma
  else
    CFLAGS_PLATFORM=-D_WIN32 -D_VCC6_
    LDFLAGS_PLATFORM=
  endif
endif

OBJECTS = $(CPP_FILES:%.cpp=$(TARGET_OUTPUT_DIRECTORY)%.o) $(C_FILES:%.c=$(TARGET_OUTPUT_DIRECTORY)%.o)
#LDFLAGS += $(SHARED_LIBRARIES:%=-l%)
# if nothing asked we do normal compilation, else we do subdirs then asked actions

MAKE_TARGETS += check_plateform do_depend-cpp

ifdef LIB_NAME
LIB_NAME := lib$(LIB_NAME).a
MAKE_TARGETS += make_library
endif # LIBRARY

ifdef EXECUTABLE_NAME
MAKE_TARGETS += make_application
endif # APPLICATION

# subdir is first if asked
ifdef SUBDIRS
MAKE_TARGETS := make_subdirs $(MAKE_TARGETS)
endif #SUBDIRS

# allow library to be considerated as dependencies !
EXPANDED_DEPLIBS=$(foreach name,$(SHARED_LIBRARIES),$(LIB_DIR)/lib$(name).a)

# not-a-file targets: 
.PHONY: subdirs $(SUBDIRS) all clean clean-java clean-cs clean-cpp clean-all gcov depend-cpp do_clean do_clean-java do_clean-cs do_clean-cpp do_clean-all do_gcov do_depend-cpp check_plateform make_library make_application make_subdirs $(EXPANDED_DEPLIBS)

all: $(MAKE_TARGETS)

clean: do_clean make_subdirs

clean-java: do_clean-java make_subdirs

clean-cs: do_clean-cs make_subdirs

clean-cpp: do_clean-cpp make_subdirs

clean-all: do_clean-all make_subdirs

gcov: do_gcov make_subdirs

depend-cpp: do_depend-cpp make_subdirs


.ONESHELL:
SHELL = /bin/bash
check_plateform:
	@if [ "$(PLATF)" == "CYGWIN_NT-5.1" ]; then echo '********************* CYGWIN *******************'; \
	else \
		if [ "$(PLATF)" == "Linux" ]; then echo '********************* LINUX *******************'; \
		else \
			echo '********************* WIN32 ? *******************'; \
		fi \
	fi

make_library: $(OBJECTS) $(LIB_NAME)

$(LIB_NAME): $(OBJECTS) $(EXPANDED_DEPLIBS)
	@echo 'Building lib $(LIB_NAME)'
	@$(AR) rvu $(LIB_NAME) $(OBJECTS) && \
	ranlib $(LIB_NAME) && \
	cp $(LIB_NAME) $(LIB_DIR)/$(LIB_NAME)
#rm -f $@.o ;\
	@$(DUM_INCL) -i "./*.h" $(INCLUDE_DIR)
	@$(DUM_INCL) -i "./*.hpp" $(INCLUDE_DIR)


make_application: $(OBJECTS) $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(OBJECTS) $(EXPANDED_DEPLIBS)
	@echo 'Building executable $(EXECUTABLE_NAME)'
	$(CC) -o $(EXECUTABLE_NAME) $(LDFLAGS) $(OBJECTS) $(SHARED_LIBRARIES:%=-l%) -lpthread  &&\
	cp $(EXECUTABLE_NAME) $(EXECUTABLE_DIR)/$(EXECUTABLE_NAME)
#rm -f $@.o ;\
	@$(DUM_INCL) -i "./*.h" $(INCLUDE_DIR)
	@$(DUM_INCL) -i "./*.hpp" $(INCLUDE_DIR)


make_subdirs:
ifdef SUBDIRS
	@$(foreach dir,$(SUBDIRS),\
	$(MAKE) -C $(dir) $(MAKECMDGOALS) &&) echo "All SUBDIRS done!"
endif

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

do_clean: do_clean-cpp do_clean-java do_clean-cs 

do_clean-java: 
	@if [ -e 'pom.xml' ]; then \
	mvn clean; \
	fi

do_clean-cs: 
	@if [ -e 'build.xml' ]; then \
	nant -D:ROOT_DIR=$(ROOT_DIR) -buildfile:build.xml clean; \
	fi

do_clean-cpp: 
	rm -f core eddep makedep *.gcda *.gcno gcov.log .depend*
	find . -iname '*.o' -exec rm -f {} \;
ifdef EXECUTABLE_NAME
	rm -f core $(EXECUTABLE_DIR)/$(EXECUTABLE_NAME) $(EXECUTABLE_NAME)
endif
ifdef LIB_NAME
	rm -f $(LIB_NAME) $(LIB_DIR)/$(LIB_NAME)
endif

do_clean-all: do_clean
	find . -iname '*.log*' -exec rm -f {} \;
	find . -iname '*.bak' -exec rm -f {} \;
	find . -iname '*~' -exec rm -f {} \;

do_gcov:
	gcov -p -a -b $(CPP_FILES) $(C_FILES) > gcov.log

do_depend-cpp: .depends

coverage:
# nop

.depends: $(CPP_FILES) $(C_FILES)
#	@echo "depend ..."
	@echo '# GENERATED FILE: ANY MODIFICATIONS WILL BE OVERRIDEN' > .depends
	@for i in $(CPP_FILES) $(C_FILES) ; do \
	  $(CCDEPEND) $$i >> .depends ;\
	done

	@if [ -e $(DUM_INCL) ]; then \
		$(DUM_INCL) -i "./*.h" $(INCLUDE_DIR); \
		$(DUM_INCL) -i "./*.hpp" $(INCLUDE_DIR); \
	fi
#	@echo "depend done !"
