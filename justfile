build:
	ninja -C build -v

run: build
	build/dankeyboard

debug:
	gdb build/dankeyboard
