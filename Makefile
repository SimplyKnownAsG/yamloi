
ifeq ($(OS),Windows_NT)
	CXX=cl
	CXXFLAGS= /nologo /EHa /fp:strict /MP "/IC:\tools\Python27\include" /I. /c $< /Fo:$@
	LDFLAGS= /nologo /incremental:no '/libpath:C:\tools\Python27\libs'
	LDSHARED=link $(LDFLAGS) /DLL /out:$@
	LDSTATIC=lib $(LDFLAGS) /out:$@
	LDEXE=link $(LDFLAGS) /out:$@
	LIBYAMLOI=$(BUILD_DIR)/lib/LIBYAMLOI.lib
	PY_EXT=$(BIN_DIR)/_yamloi.pyd
else
	CXX=g++
	LD=g++
	CXXFLAGS= -fPIC -Wall -std=c++1y -c $< -o $@ -I/usr/include/python2.7
	LDFLAGS= -fPIC
	LDSHARED=g++ -Wall -shared -o $@
	LDSTATIC=ar crs $@
	LDEXE=g++ -Wall -o $@
	LIBYAMLOI=$(BUILD_DIR)/lib/LIBYAMLOI.a
	PY_EXT=$(BIN_DIR)/_yamloi.so
endif

ifeq ($(MAKECMDGOALS),debug)
	BUILD_DIR=debug
	CXXFLAGS+= -g
	LDFLAGS+= -g
else
	CXXFLAGS+= -O2
	BUILD_DIR=release
endif

# for standard CXX settings
SRC_DIR=yamloi
OBJ_DIR=$(BUILD_DIR)/obj
BIN_DIR=$(BUILD_DIR)/bin

# functions
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
define colorecho
	@tput setaf 4
	@echo $1
	@tput sgr0
	@$1

endef

SRC = $(filter-out %_wrap.cpp,$(call rwildcard, $(SRC_DIR), *.cpp))
HEADERS = $(filter-out %_wrap.hpp yamloi/yamloi.hpp, $(call rwildcard, $(SRC_DIR), *.hpp))
DIRECTORIES = $(sort $(dir $(SRC) $(HEADERS)))
OBJ = $(filter-out %_wrap.obj,$(patsubst %.cpp,$(OBJ_DIR)/%.obj,$(SRC)))

.PHONY: test test_py test_cpp

##
## BEGIN DIRECTORY MAGIC
##
get_parent_dir=$(dir $(patsubst %/,%,$1))

TARGET_DIRS:=
define make_dirs_recursively
TARGET_DIRS+=$1
$1: | $(if $(subst ./,,$(call get_parent_dir,$1)),$(call get_parent_dir,$1))
	mkdir $1
endef

get_all_dirs=$(if $(subst ./,,$(dir $1)),$(call get_all_dirs,$(call get_parent_dir,$1)) $1)

get_all_target_dirs=$(sort $(foreach target,$1,$(call get_all_dirs,$(dir $(target)))))

create_dirs=$(foreach dirname,$(call get_all_target_dirs,$1),$(eval $(call make_dirs_recursively,$(dirname))))
##
## END DIRECTORY MAGIC
##


all: release

$(call create_dirs,$(OBJ) $(PY_EXT) $(LIBYAMLOI))

release: test examples

debug: test examples

clean:
	-$(RM) -r ./debug/ ./release/
	-$(RM) ./yamloi/yamloi.hpp
	-$(RM) ./yamloi/*wrap*

clobber:
	-git clean -fxd

yamloi/yamloi.hpp: $(HEADERS)
	@echo "// this file is auto-generated, do not modify" > $@
	@$(foreach hpp,$^,echo '#include "$(hpp)"' >> $@;)
	@echo "" >> $@

$(LIBYAMLOI): $(OBJ)
	$(call colorecho,$(LDSTATIC) $^)

$(OBJ_DIR)/%.obj: %.cpp %.hpp | $(TARGET_DIRS)
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

test: test_cpp test_py

examples: cpp_examples py_examples

# cpp targets
cpp: test_cpp cpp_examples

test_cpp: $(sort $(patsubst %.cpp, $(BIN_DIR)/%.exe, $(call rwildcard, tests, *.cpp)))
	$(foreach cpp_test_exec, $^, $(call colorecho,$(cpp_test_exec)))

cpp_examples: $(sort $(patsubst %.cpp, $(BIN_DIR)/%.exe, $(call rwildcard, examples, *.cpp)))
	$(foreach cpp_test_exec, $^, $(call colorecho,$(cpp_test_exec)))

$(BIN_DIR)/%.exe: yamloi/yamloi.hpp $(OBJ_DIR)/%.obj $(LIBYAMLOI) | $(TARGET_DIRS)
	$(call colorecho,$(LDEXE) $(filter-out yamloi/yamloi.hpp,$^))

$(OBJ_DIR)/%.obj: %.cpp | $(TARGET_DIRS)
	$(call colorecho,$(CXX) $(CXXFLAGS) -I.)

# Python targets
py: test_py py_examples

test_py: PY_TEST_FILES=$(sort $(call rwildcard, tests, *.py))
test_py: $(PY_EXT)
	$(foreach pytestfile, $(PY_TEST_FILES), $(call colorecho,PYTHONPATH=$(BIN_DIR) python $(pytestfile)))

py_examples: PY_EXAMPLE_FILES=$(sort $(call rwildcard, examples, *.py))
py_examples: $(PY_EXT)
	$(foreach pytestfile, $(PY_EXAMPLE_FILES), $(call colorecho,PYTHONPATH=$(BIN_DIR) python $(pytestfile)))

$(PY_EXT): $(LIBYAMLOI) $(OBJ_DIR)/yamloi/yamloi_wrap.obj | $(TARGET_DIRS)
	@# $< is repeated (implicit in $^) because the symbols are only needed after yamloi_wrap.obj
	$(call colorecho,$(LDSHARED) $^ $<)

$(SRC_DIR)/yamloi_wrap.cpp $(SRC_DIR)/yamloi_wrap.hpp: yamloi.i yamloi/yamloi.hpp | $(TARGET_DIRS)
	$(call colorecho,swig -python -builtin -includeall -ignoremissing -c++ -outdir $(BIN_DIR) -o $(SRC_DIR)/yamloi_wrap.cpp -oh $(SRC_DIR)/yamloi_wrap.hpp yamloi.i)

