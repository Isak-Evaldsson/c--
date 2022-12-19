.PHONY: test

builddir:
	meson setup builddir

I-Lib/build/libi.a:
	cd I-Lib && make	

test: builddir I-Lib/build/libi.a
	cd builddir && meson test --wrap='valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=1'
