/* global ImageData, AudioContext, AudioWorkletNode */

// I inline worker, so no seperate file is needed.
const workletUrl = URL.createObjectURL(new Blob([`

/* global AudioWorkletProcessor, registerProcessor, currentFrame, currentTime, sampleRate  */

class WebDevFSDsp extends AudioWorkletProcessor {
  constructor (...args) {
    super(...args)
    this.port.onmessage = e => this.buffer = e.data
  }

  // STATIC
  // process (inputs, outputs, parameters) {
  //   outputs[0].forEach((channel) => {
  //     for (let i = 0; i < channel.length; i++) {
  //       channel[i] = Math.random() * 2 - 1 * parameters.gain[0]
  //     }
  //   })
  //   return true
  // }

  process (inputs, outputs, parameters) {
    if (this.buffer) {
      const c = currentFrame % sampleRate
      outputs[0][0].set(this.buffer.slice(c, c+128))
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

`], { type: 'application/javascript' }))

export class WebDevFS {
  framebuffer (canvas) {
    if (typeof canvas === 'undefined') {
      canvas = document.createElement('canvas')
      document.body.appendChild(canvas)
    }
    this.canvas = canvas
    this.ctx = canvas.getContext('2d')
    return {
      name: 'framebuffer',
      isBuffered: false,
      read () {},
      write: this.write.bind(this)
    }
  }

  dsp (audioCtx = new AudioContext()) {
    this.audioCtx = audioCtx
    this.audioBuffer = new ArrayBuffer(audioCtx.sampleRate * 4)

    audioCtx.audioWorklet.addModule(workletUrl).then(() => {
      this.dsp = new AudioWorkletNode(
        audioCtx,
        'webdevfs-dsp'
      )
      this.dsp.connect(audioCtx.destination)
      this.dsp.port?.postMessage(this.audioBuffer)
    })

    // add a click-handler to resume (due to web security) https://goo.gl/7K7WLu
    document.addEventListener('click', () => {
      if (this.audioCtx.state !== 'running') {
        this.audioCtx.resume()
      }
    })

    return {
      name: 'dsp',
      isBuffered: false,
      read () {},
      write: this.write.bind(this)
    }
  }

  write ({ device: { driver: { name }, ino }, fs, path, position }, data) {
    // TODO: these are rudimentary, and do not account for position

    if (name === 'framebuffer' && data.byteLength && this.canvas) {
      const imageData = new ImageData(new Uint8ClampedArray(data), this.canvas.width, this.canvas.height)
      this.ctx.putImageData(imageData, 0, 0)
    }
    if (name === 'dsp' && data.byteLength && this.audioBuffer) {
      new Uint8Array(this.audioBuffer).set(data)
      this.dsp.port?.postMessage(new Float32Array(this.audioBuffer))
    }
  }
}
export default WebDevFS
