This library allows you to easily make games in WASI that use 2D graphics, sound, and input. It's meant to be a light alternative to emscripten/SDL, but also allows languages other than C to target the web.

I wanted to be able to use my [easywasi](https://github.com/konsumer/easywasi) with [zenfs](https://www.npmjs.com/package/@zenfs/core) to output graphics/sound and read input, over WASI fs.

It will run on web or anywhere else with js/canvas/audiocontext, like nodejs, via other libraries.

Also check out [@zen-fs/devices](https://github.com/zen-fs/devices). It's a typescript-focused soft-fork of the work here.

## demos

- [static](https://konsumer.js.org/web-zen-dev/static)
- [drawing with pntr](https://konsumer.js.org/web-zen-dev/pntr)
- [sound](https://konsumer.js.org/web-zen-dev/sound)

## devices

These are the devices I am implementing here:

- `/dev/dsp` - sound (similar to OSS)
- `/dev/fb0` - 2D graphics (framebuffer)
- `/dev/input/*` - input devices (keyboard, mouse, gamepad)

and maybe this:

- `/proc/bus/input/devices` - get list of input devices

additionally, zenfs already provides these, if you use `addDevices:true`:

```
/dev/null
/dev/zero
/dev/full
/dev/random
```

## usage

You can use it like this:

```js
import { configure, fs } from '@zenfs/core'
import { framebuffer, dsp } from 'web-zen-dev'

await configure({
  addDevices: true
})

fs.mounts.get('/dev').createDevice('/fb0', framebuffer())
fs.mounts.get('/dev').createDevice('/dsp', await dsp())
```

You can see an example [here](docs/).
