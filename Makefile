CC = clang

WFLAGS = -Os -W -Wall
SYSFLAGS = --target=wasm32-wasi --sysroot $(PWD)/$(BUILD) -nostartfiles
LDFLAGS = -Wl,--import-memory -Wl,--no-entry -Wl,--export-all
CFLAGS = $(WFLAGS) $(SYSFLAGS) $(LDFLAGS)
LIBS =

BUILD = wasi-libc-build

%.wasm: %.c $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

all: copy.wasm canvas.wasm
	@echo "Done"
	@echo "start local webserver with: python -m http.server"
	@echo "and visit - http://localhost:8000/"

$(BUILD):
	make -C wasi-libc install INSTALL_DIR=../$(BUILD)
	touch $@

.PHONY: clean

clean:
	rm -f *.wasm *~

distclean: clean
	rm -rf $(BUILD)
	@cd wasi-libc; git clean -fdx
