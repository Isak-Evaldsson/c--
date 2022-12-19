.PHONY: test

builddir:
	meson setup builddir

I-Lib/build/libi.a:
	cd I-Lib && make	

test: builddir I-Lib/build/libi.a
	cd builddir && meson test
