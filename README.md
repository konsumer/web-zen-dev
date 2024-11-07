This library allows you to easily make games in WASI that use 2D graphics, sound, and input. It's meant to be a light alternative to emscripten/SDL, but also allows languages otehr than C to target the web.

It will run on web or anywhere else with js/canvas/audiocontext, like nodejs, via other libraries.

I wanted to be able to use my [easywasi](https://github.com/konsumer/easywasi) with [zenfs](https://www.npmjs.com/package/@zenfs/core) to output graphics/sound and read input, over WASI fs.

Also check out [@zen-fs/devices](https://github.com/zen-fs/devices). It's a typescript-focused soft-fork of the work here.

## demos

- [static](https://konsumer.js.org/web-zen-dev/static)
- [drawing with pntr](https://konsumer.js.org/web-zen-dev/pntr)
- [sound](https://konsumer.js.org/web-zen-dev/sound)

## devices

These are the devices I am implementing here:

- `/dev/dsp` - sound (similar to OSS)
- `/dev/fb0` - 2D graphics (framebuffer)
- `/dev/input/*` - actual input devices

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
import { configure, InMemory, fs } from '@zenfs/core'
import { IndexedDB } from '@zenfs/dom'
import WebDevFS from 'web-zen-dev'

await configure({
  addDevices: true
})

const wd = new WebDevFS()
fs.mounts.get('/dev').createDevice('/fb0', wd.framebuffer())
fs.mounts.get('/dev').createDevice('/dsp', wd.dsp())
```

You can see an example [here](docs/index.html).
