This is a native host for devices, implemented in [wamr](https://github.com/bytecodealliance/wasm-micro-runtime).

The purpose of this is to make games that don't need recompile, in WASM (so any language that supports outputting that) that run on web & native, that use only these simple device-files (via WASI):

- `/dev/dsp` - sound (similar to OSS)
- `/dev/fb0` - 2D graphics (framebuffer)
- `/dev/input/*` - input devices (keyboard, mouse, gamepad)

## building

```
cmake -B build
cmake --build build
```

## usage

```
./build/host ../docs/pntr/test.wasm
```