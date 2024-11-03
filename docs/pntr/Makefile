WASI_SDK_ROOT:=/opt/wasi-sdk

test.wasm: src/test.c
	${WASI_SDK_ROOT}/bin/clang -O3 --sysroot=${WASI_SDK_ROOT}/share/wasi-sysroot -o $@ $<