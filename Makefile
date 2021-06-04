CC = clang

CFLAGS = --target=wasm32-wasi --sysroot $(PWD)/$(BUILD) -nostartfiles -Wl,--import-memory -Wl,--no-entry -Wl,--export-all
LDFLAGS =

BUILD = wasi-libc-build

%.wasm: %.c $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

all: copy.wasm
	@echo "$< ready"
	@echo "start local webserver wih python -m http.server"
	@echo "and visit http://localhost:8000/copy.html"

$(BUILD):
	make -C wasi-libc install INSTALL_DIR=../$(BUILD)
	touch $@

.PHONY: clean

clean:
	rm -f *.wasm *~

distclean: clean
	rm -rf $(BUILD)
	@cd wasi-libc; git clean -fdx
