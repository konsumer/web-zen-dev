WASI_SDK_ROOT:=/opt/wasi-sdk

../docs/pntr/test.wasm: src/test.c
	${WASI_SDK_ROOT}/bin/clang -O3 --sysroot=${WASI_SDK_ROOT}/share/wasi-sysroot -D_WASI_EMULATED_MMAN -lwasi-emulated-mman -o $@ $<