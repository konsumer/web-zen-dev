> [!WARNING]  
> Development has moved to [@zen-fs/devices](https://github.com/zen-fs/devices). This is where I worked our original ideas, but you should use that instead.

Write old-linux-style code in any language that targets wasm/wasi, and run it on the web (or anywhere else with js.)

I wanted to be able to use my [easywasi](https://github.com/konsumer/easywasi) with [zenfs](https://www.npmjs.com/package/@zenfs/core) to output graphics/sound and read input, over WASI fs.

## demos

- [static](https://konsumer.js.org/web-zen-dev/)
- [drawing](https://konsumer.js.org/web-zen-dev/pntr)

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
