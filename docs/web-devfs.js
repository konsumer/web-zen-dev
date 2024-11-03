/* global ImageData, AudioContext, AudioWorkletNode */

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

    // TODO: inline this, so a seperate file is not needed
    audioCtx.audioWorklet.addModule('webdevfs-dsp.js').then(() => {
      this.dsp = new AudioWorkletNode(
        audioCtx,
        'webdevfs-dsp'
      )
      this.dsp.connect(audioCtx.destination)
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
    if (name === 'dsp' && data.byteLength && this.audioCtx) {
      this?.dsp?.port?.postMessage(data)
    }
  }
}
export default WebDevFS
