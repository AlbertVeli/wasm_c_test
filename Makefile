CC = clang

WFLAGS = -Os -W -Wall
SYSFLAGS = --target=wasm32-wasi --sysroot $(PWD)/$(BUILD) -nostartfiles
LDFLAGS = -Wl,--import-memory -Wl,--no-entry -Wl,--export-all
CFLAGS = $(WFLAGS) $(SYSFLAGS) $(LDFLAGS)
LIBS =

BUILD = wasi-libc-build

%.wasm: %.c $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

all: copy.wasm canvas.wasm fire.wasm
	@echo "Done"
	@echo "start local webserver with: python -m http.server"
	@echo "and visit - http://localhost:8000/"

$(BUILD): wasi-libc
	make -C wasi-libc install INSTALL_DIR=../$(BUILD)
	touch $@

wasi-libc:
	git submodule update --init --recursive

createvideo:
	make -f Makefile.createvideo
	# Could insert convert + ffmpeg commands here
	# see output from createvideo

.PHONY: clean

clean:
	rm -f *.wasm *~
	make -f Makefile.createvideo clean

distclean: clean
	rm -rf $(BUILD)
	@cd wasi-libc; git clean -fdx
