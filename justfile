build:
	ninja -C build -v

run: build
	build/dankeyboard

debug: build
	gdb build/dankeyboard
