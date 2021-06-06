WebAssembly test. C -> WebAssembly w/o Emscripten.
Using only llvm + clang with WebAssembly target.

Examples
 * copy example is from this [article](https://depth-first.com/articles/2019/10/16/compiling-c-to-webassembly-and-running-it-without-emscripten/) + some glue
 * canvas example is inspired by this [article](https://compile.fi/canvas-filled-three-ways-js-webassembly-and-webgl/) (but without emscripten)
 * Fire is very similar to canvas except it actually draws something on the canvas. A classic fire effect.

[![Fire demo](https://img.youtube.com/vi/BzRzM3JZX9g/0.jpg)](https://www.youtube.com/watch?v=BzRzM3JZX9g)

Requirements:
 * llvm / clang with target WebAssembly

On gentoo, add to *package.use*:

```
*/* LLVM_TARGETS: WebAssembly
```

Also enable use-flag *default-lld* to depend on *sys-devel/lld*, or install
it separately.

If you get an error like:

```
wasm-ld: error: cannot open /usr/lib/clang/11.1.0/lib/wasi/libclang_rt.builtins-wasm32.a: No such file or directory
```

You need to install *libclang_rt_builtins-wasm32.a*. The easiest is to install
it pre-compiled from (at the time of this writing *wasi-sdk-12* is the current
version, pick the latest)

 https://github.com/WebAssembly/wasi-sdk/releases/

The missing file is in the *libclang_rt.builtins-wasm32-wasi-12.0.tar.gz*
gzip. Unpack it and copy the .a file to where clang wants it to be.
In my case */usr/lib/clang/11.1.0/lib/wasi/*. I had to create the wasi
directory first.

Another option is to compile the entire wasi-sdk from the same page
or download a pre-compiled wasi-sdk toolchain.

To try it out, start a test webserver with:

```python -m http.server```

and visit http://localhost:8000/

copy.html writes to the javascript console. canvas.html and
fire.html draws pixels to a C-array which is copied to a HTML
canvas object by javascript.

If everything works you can start writing your own WebAssembly
apps - in C - without Emscripten.

```
Albert Teobas Veli
Setting Orange,
the 9th day of Confusion in the Year of Our Lady of Discord 3187

       ~~=) All Rights Reversed - No Rights Reserved (=~~
```
