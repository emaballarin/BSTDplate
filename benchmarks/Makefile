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
SRC = benchmarks.cpp
################################################################################

##############
## GNU C.C. ##
##############
#CXX = g++
#CXXFLAGS = -Ofast -flto -march=native -W -Wall -Wextra -std=c++17 -fassociative-math -fcode-hoisting -fexpensive-optimizations -ffast-math -ffinite-math-only -fhoist-adjacent-loads -findirect-inlining -floop-nest-optimize -fpartial-inlining -fpeel-loops -fpeephole -fpeephole2 -fprefetch-loop-arrays -freciprocal-math -frename-registers -funswitch-loops -funsafe-math-optimizations -I./ -I../include/


###############
## INTEL C.C ##
###############
CXX = icpc
CXXFLAGS = -std=c++17 -xCORE-AVX2 -xHost -fma -use-intel-optimized-headers -qopt-subscript-in-range -falign-loops -parallel -funroll-loops -ipo -vec -no-inline-max-size -no-inline-max-total-size -W -Wall -Wextra -Ofast -I./ -I../include/


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

valgrind:
	make all
ifneq ("$(wildcard ./*.x)", "")
	@valgrind -s ./*.x || echo "Please, install valgrind to run this command!"
endif

.PHONY: valgrind

clean:
	rm -f $(EXE) *~ ./results/*

.PHONY: clean
