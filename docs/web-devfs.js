/* global ImageData, AudioContext, AudioWorkletNode */

// I inline worker, so no seperate file is needed for worker
const workletUrl = URL.createObjectURL(
  new Blob(
    [
      `

/* global AudioWorkletProcessor, registerProcessor, currentFrame, currentTime, sampleRate  */

class WebDevFSDsp extends AudioWorkletProcessor {
  constructor (...args) {
    super(...args)
    this.buffer = new Float32Array()
    this.port.onmessage = e => {
      this.buffer = new Float32Array(e.data)
    }
  }

  process (inputs, outputs, parameters) {
    if (this.buffer.byteLength >= 128) {
      outputs[0][0].set(this.buffer.slice(0, 128))
      this.buffer = this.buffer.slice(128)
    }
    return true
  }

  static get parameterDescriptors () {
    return [
      {
        name: 'gain',
        defaultValue: 1,
        minValue: 0,
        maxValue: 1,
        automationRate: 'a-rate'
      }
    ]
  }
}

registerProcessor('webdevfs-dsp', WebDevFSDsp)

`
    ],
    { type: 'application/javascript' }
  )
)

export async function dsp({ audioContext = new AudioContext() }) {
  await audioContext.audioWorklet.addModule(workletUrl)
  const dsp = new AudioWorkletNode(audioContext, 'webdevfs-dsp')
  dsp.connect(audioContext.destination)

  // add a click-handler to resume (due to web security) https://goo.gl/7K7WLu
  document.addEventListener('click', () => {
    if (audioContext.state !== 'running') {
      audioContext.resume()
    }
  })

  return {
    name: 'dsp',
    isBuffered: false,
    audioContext,
    read() {},
    write({ device: { driver: { name }, ino }, fs, path, position }, data) {
      if (data.byteLength) {
        dsp.port?.postMessage(data.buffer)
      }
    }
  }
}

export function framebuffer({ canvas }) {
  if (!canvas) {
    canvas = document.createElement('canvas')
    document.body.appendChild(canvas)
  }
  const ctx = canvas.getContext('2d')
  return {
    name: 'framebuffer',
    isBuffered: false,
    canvas,
    read() {},
    write({ device: { driver: { name }, ino }, fs, path, position }, data) {
      if (data?.length) {
        const imageData = new ImageData(new Uint8ClampedArray(data), canvas.width, canvas.height)
        ctx.putImageData(imageData, 0, 0)
      }
    }
  }
}
