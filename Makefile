# Simple Makefile to compile, clean and format all the C++ code contained in the
# repository.

default: all

all: compile

bear: clean compile

compile:
	+$(MAKE) $@ -C src -f Makefile
	+$(MAKE) $@ -C include -f Makefile
	+$(MAKE) $@ -C benchmarks -f Makefile

clean:
	+$(MAKE) $@ -C src -f Makefile clean
	+$(MAKE) $@ -C include -f Makefile clean
	+$(MAKE) $@ -C benchmarks -f Makefile clean
	@find . -type f \( -name '*~' -o -name 'a.out' -name '*.x' \) -exec rm '{}' \;

format:
	+make clangify
	+$(MAKE) $@ -C src
	+$(MAKE) $@ -C include
	+$(MAKE) $@ -C benchmarks
	+make clangify-revert

clangify:
	+find . -type d -exec cp -f "./.clang-format" {} \;

docs: ./doc/doxy.in
	doxygen $^

clangify-revert:
	+cp -f ./.clang-format ./.clang-format-backup
	+find . -type d -exec rm -R -f {}/./.clang-format \;
	+mv -f ./.clang-format-backup ./.clang-format

# May not work if you don't have the right files/tools/executables in-place...
pvsrun:
	+rm -f ./compile_commands.json
	+bear make bear
	+make clean
	+rm -f ./project.log
	+rm -f ./project.tasks
	+rm -f ./strace_out
	+pvsdeploy add ./
	+pvs-studio-analyzer trace -- make bear
	+pvs-studio-analyzer analyze -o ./project.log -j4
	+plog-converter -a GA:1,2 -t tasklist -o ./project.tasks ./project.log
	+pvsdeploy rem ./

# Just like the one above, but working only if you have a "real" license. Avoids some "jumping through hoops of fire", OTOH.
# May not work if you don't have the right files/tools/executables in-place...
pvsrun-compliant:
	+rm -f ./compile_commands.json
	+bear make bear
	+make clean
	+rm -f ./project.log
	+rm -f ./project.tasks
	+rm -f ./strace_out
	+pvs-studio-analyzer trace -- make bear
	+pvs-studio-analyzer analyze -o ./project.log -j4
	+plog-converter -a GA:1,2 -t tasklist -o ./project.tasks ./project.log

# May not work if you don't have the right files/tools/executables in-place...
bear-aio:
	+make clean
	+bear make bear
	+make clean

# May not work if you don't have the right files/tools/executables in-place...
clean-all:
	+make clean
	+rm -f ./project.log
	+rm -f ./project.tasks
	+rm -f ./strace_out
	+pvsdeploy rem ./


.PHONY: all bear compile clean format default clangify clangify-revert pvsrun bear-aio clean-all pvsrun-compliant docs
