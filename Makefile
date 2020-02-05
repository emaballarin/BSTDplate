# Simple Makefile to compile, clean and format all the C++ code contained in the
# repository.

default: all

all: compile

bear: clean compile

compile:
	+$(MAKE) $@ -C src -f Makefile
	+$(MAKE) $@ -C include -f Makefile

clean:
	+$(MAKE) $@ -C src -f Makefile clean
	+$(MAKE) $@ -C include -f Makefile clean
	@find . -type f \( -name '*~' -o -name 'a.out' -name '*.x' \) -exec rm '{}' \;

format:
	+$(MAKE) $@ -C src
	+$(MAKE) $@ -C include

clangify:
	+find . -type d -exec cp -f "./.clang-format" {} \;

clangify-revert:
	+cp -f ./.clang-format ./.clang-format-backup
	+find . -type d -exec rm -R -f {}/./.clang-format \;
	+mv -f ./.clang-format-backup ./.clang-format

# May not work if you don't have the right files/tools/executables in-place...
pvsrun:
	+rm ./compile_commands.json
	+bear make bear
	+make clean
	+rm ./project.log
	+rm ./project.tasks
	+rm ./strace_out
	+pvsdeploy add ./
	+pvs-studio-analyzer trace -- make bear
	+pvs-studio-analyzer analyze -o ./project.log -j4
	+plog-converter -a GA:1,2 -t tasklist -o ./project.tasks ./project.log
	+pvsdeploy rem ./

# May not work if you don't have the right files/tools/executables in-place...
bear-aio:
	+make clean
	+bear make bear
	+make clean

# May not work if you don't have the right files/tools/executables in-place...
clean-all:
	+make clean
	+rm ./project.log
	+rm ./project.tasks
	+rm ./strace_out
	+pvsdeploy rem ./


.PHONY: all bear compile clean format default clangify clangify-revert pvsrun bear-aio clean-all
