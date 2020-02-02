# Simple Makefile to compile, clean and format all the C++ code contained in the
# repository.

default: all

all:
	+$(MAKE) $@ -C src
	+$(MAKE) $@ -C include

clean:
	+$(MAKE) $@ -C src
	+$(MAKE) $@ -C include
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

.PHONY: all clean format default clangify clangify-revert
