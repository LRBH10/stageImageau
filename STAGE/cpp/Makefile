#
# needs : libssl-dev, Socket, libxml2-dev, cppunit
#

SUBDIRS=dum_incl libcommon systemcall smdmonitor

#fake target: only used when ROOT_DIR is badly defined, otherwise the next line (include ...) overwrite the "all:" rule.
#all: root_dir_check

include $(ROOT_DIR)/Rules.make

.ONESHELL:
SHELL = /bin/bash
root_dir_check:
	@echo "ROOT_DIR env var: '$(ROOT_DIR)'"
	@if [ '$(ROOT_DIR)' == "`pwd`" ]; then echo 'ROOT_DIR correctly set'; \
	else \
        echo '!!! ROOT_DIR env var badly set !!!!';\
	echo 'Try in a shell: export ROOT_DIR=`pwd`'; \
	fi

clean-all: clean-crash clean-db clean
	find . -iname '*.bak' -exec rm -f {} \;
	find . -iname '*~' -exec rm -f {} \;
	rm -f $(ROOT_DIR)/Rules_opt.make
	touch $(ROOT_DIR)/Rules_opt.make
	rm -f imageau_*.info 
	rm -f include/*.h
	rm -f include/*.hpp
	find $(ROOT_DIR) -iname 'lib*.[a|s]*' -exec rm {} \;


# remove temporary working resources
clean-crash:
#	rm -f .eclipse_workspace/imageau_cpp/*.idx
#	rm -f .eclipse_workspace/imageau_cpp/*.cnt	
#	rm -f .eclipse_workspace/imageau_cpp/*.pid
#	rm -rf .eclipse_workspace/imageau_cpp/root
#	rm -rf .eclipse_workspace/imageau_cpp/projection
#	rm -f *.idx
#	rm -f *.cnt
#	rm -f *.pid
#	rm -rf projection/
#	rm -rf root/

clean-db:
#	-mysql -b baseTest -e "drop table LinkName; drop table Node; drop table Link;"

gcovMakefile:
	cat $(ROOT_DIR)/Rules_gcov.make >> $(ROOT_DIR)/Rules_opt.make


gprofMakefile:
	cat $(ROOT_DIR)/Rules_gprof.make >> $(ROOT_DIR)/Rules_opt.make


runtest: runtest-cpp runtest-java


runtest-cpp: 
	make all
	find $(ROOT_DIR) -type f -iname 'imageau*.info' -exec rm -f {} \;
	-lcov -c -i -d $(ROOT_DIR)/ -o $(ROOT_DIR)/imageau_base.info
	-lcov -c -i -d $(ROOT_DIR)/ -o $(ROOT_DIR)/imageau_base.info # done twice to avoid error
	
	find $(ROOT_DIR) -type f -iname 'cppunit_*.xml' -exec rm -f {} \;

	-for i in `find $(ROOT_DIR)/bin -iname 'TestRunner*'`; do cd $(ROOT_DIR)/dist; sudo $$i 2> /tmp/test_out_err.txt ; done
	 
	cd $(ROOT_DIR)/dist
	$(ROOT_DIR)/bin/merge_cppunit_result.sh $(ROOT_DIR)/dist/cppunit_merged_result.xml
	xsltproc -o $(ROOT_DIR)/doc/cppunit_result.html $(ROOT_DIR)/doc/cppunit_report.xsl $(ROOT_DIR)/dist/cppunit_merged_result.xml

	cd $(ROOT_DIR)


runtest-java:
	#mvn test


BAD_COV_DIR=/usr/include/boost /usr/include/boost/detail /usr/include/boost/multi_index /usr/include/boost/multi_index/detail /usr/include/boost/tuple/detail 
BAD_COV_DIR+=/usr/include/boost/utility /usr/include/boost/bind /usr/include/boost/interprocess /usr/include/boost/interprocess/detail 
BAD_COV_DIR+=/usr/include/boost/interprocess/sync /usr/include/boost/interprocess/sync/posix /usr/include/boost/smart_ptr /usr/include/boost/smart_ptr/detail
BAD_COV_DIR+=/usr/include/boost/exception /usr/include/boost/regex /usr/include/boost/regex/v4

BAD_COV_DIR+=/usr/include/sys /usr/include/c++/4.5 /usr/include/c++/4.5/backward /usr/include/c++/4.5/bits /usr/include/c++/4.5/ext /usr/include/c++/4.5/x86_64-linux-gnu/bits 
BAD_COV_DIR+=/usr/include/c++/4.5/i486-linux-gnu/bits

BAD_COV_DIR+=/usr/include/log4cxx /usr/include/log4cxx/helpers 
BAD_COV_DIR+=/usr/include/cppunit /usr/include/cppunit/extensions /usr/include/cppunit/portability /usr/local/include/Sockets /usr/local/include/mysql++ 
BAD_COV_DIR+=/usr/include/valgrind /usr/include/qt4/QtCore /usr/include/qt4/QtDesigner /usr/include/qt4/QtGui /usr/include/FTGL /usr/include/mysql++


coverage: clean-cpp clean-crash gcovMakefile runtest   
	# reset Rules_opt 
	rm -f $(ROOT_DIR)/Rules_opt.make 
	touch $(ROOT_DIR)/Rules_opt.make
	
	# clean doc dir
	mkdir -p $(ROOT_DIR)/doc/coverage 
	rm -rf $(ROOT_DIR)/doc/coverage/*

	# generate lcov after test executions
	lcov -c -d $(ROOT_DIR)/ -o $(ROOT_DIR)/imageau_test.info 
	lcov -c -d $(ROOT_DIR)/ -o $(ROOT_DIR)/imageau_test.info # done twice to avoid error
	# compute diff 		
	lcov -a $(ROOT_DIR)/imageau_base.info -a $(ROOT_DIR)/imageau_test.info -o $(ROOT_DIR)/imageau_total.info

	# remove useless dir
	for dir in $(BAD_COV_DIR); do lcov -r $(ROOT_DIR)/imageau_total.info $$dir/* -o $(ROOT_DIR)/imageau_total.info; done

	# generate html report
	genhtml --sort --function-coverage --branch-coverage --demangle-cpp --legend --show-details -o $(ROOT_DIR)/doc/coverage $(ROOT_DIR)/imageau_total.info
	
	# cleaning ...
	find $(ROOT_DIR) -iname '*.gc*' -exec rm -f {} \;	
	cd $(ROOT_DIR)
	$(MAKE) clean-cpp
	
	# update index.html file
	DATE=`date`
	cat $(ROOT_DIR)/doc/index.html | sed 's/\(Last modified:.*\$\)/Last modified: '"$(DATE)"'/g' > $(ROOT_DIR)/doc/index.html.tmp
	mv $(ROOT_DIR)/doc/index.html.tmp $(ROOT_DIR)/doc/index.html
	@echo ""
	@echo "Check file:///$(ROOT_DIR)/doc/index.html in a browser."


profile: clean clean-crash gprofMakefile runtest
	rm -f $(ROOT_DIR)/Rules_opt.make
	touch $(ROOT_DIR)/Rules_opt.make
	rm -f $(ROOT_DIR)/doc/profile.txt

	gprof $(ROOT_DIR)/bin/Debug/TestRunner > $(ROOT_DIR)/doc/profile.txt
	@echo ""
	@echo "Check file:///$(ROOT_DIR)/doc/index.html in a browser."


doc-only:
	mkdir -p $(ROOT_DIR)/doc/cppdoc
	rm -rf $(ROOT_DIR)/doc/cppdoc/*
	doxygen $(ROOT_DIR)/imageau.doxyfile
ifeq ($(shell ls -1 pom.xml 2>/dev/null),pom.xml)
	rm -rf $(ROOT_DIR)/doc/mavendoc
	mvn site
	mv target/site $(ROOT_DIR)/doc/mavendoc
	mv src/common/target/site $(ROOT_DIR)/doc/mavendoc/Common
else
	@echo "No pom.xml ==> no maven doc generation"
endif


doc: doc-only coverage


help: root_dir_check
	@echo "Available target :"
	@echo " all: compile all progs"
	@echo " all-cpp: compile all cpp progs"
	@echo " all-java: compile all java progs"
	@echo " all-cs: compile all csharp progs"
	@echo " clean: clean only compiled files"
	@echo " clean-crash: remove test created files"
	@echo " clean-db: clean database entries"
	@echo " clean-all: do clean, clean-crash, remove backup and info. BUT 'depend' must be run after if a new compilation is needed."
	@echo " clean-cpp: clean only cpp compiled files"
	@echo " clean-java: clean only java compiled files"
	@echo " clean-cs: clean only csharp compiled files"
	@echo " depend: add depend info in makefiles"
	@echo " depend-cpp: add depend info in makefiles for cpp"
	@echo " runtest: run the tests"
	@echo " runtest-cpp: run the tests for cpp"
	@echo " runtest-java: run the tests for java"
	@echo " coverage: generate test coverage documentation"
	@echo " doc-only: doxygen doc"
	@echo " doc: coverage and doc-only"
	@echo " profile: generate profiling information"

dist: $(SUBDIRS)
	find bin -type f -executable | xargs cp -t dist 

remote:
	ssh remgeau@192.168.75.51 -C "cd prog/new_station_from_uml/ && ./maker.sh"
