# GUIDELINES:
# - Use .cpp as the extension for C++ source files (non-header)
# - Use .h and/or .hpp as the extension for C++ header files
# - Preferrably, put C++ header files in /include/ and source files in /src/
# - The parts more likely to be modified are enclosed in `#`s: `SRC =` and the
#   lines at the end of the file. Add local (in-folder) files, to be compiled,
#   to the `SRC =` variable, by using the `\` endline character if willing to
#   use multiple lines. Specify per-file options in the final lines of the
#   makefile.
# - Be compliant to no more than C++17 (plus GNU extensions / C11 backports)

default: all

################################################################################
SRC =
################################################################################

CXX = g++
CXXFLAGS = -W -Wall -Wextra -std=c++17 -O3 -I./ -I../include/

EXE = $(SRC:.cpp=.x)

# eliminate default suffixes
.SUFFIXES:
SUFFIXES =

# just consider our own suffixes
.SUFFIXES: .cpp .h .hpp .x .out .a .so

all: compile

.PHONY: all

compile: $(EXE)

.PHONY: compile

%.x: %.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

format: $(SRC)
	@clang-format -i $^ -verbose || echo "Please, install clang-format to run this command!"

.PHONY: format

documentation: ../doc/doxy.in
	doxygen $^

clean:
	rm -f $(EXE) *~

.PHONY: clean

################################################################################
#test.x : CXXFLAGS += -Wno-unused-variable
################################################################################
