// this is an audio-worker, and this file should be available on your webserver at webdevfs-dsp.js

/* global AudioWorkletProcessor, registerProcessor, currentFrame, currentTime, sampleRate  */

class WebDevFSDsp extends AudioWorkletProcessor {
  constructor (...args) {
    super(...args)
    this.buffer = new Float32Array(sampleRate)
    this.port.onmessage = (e) => {
      this.buffer.set(new Float32Array(e.data))
    }
  }

  process (inputs, outputs, parameters) {
    outputs[0].forEach((channel) => {
      console.log(channel.length)
      for (let i = 0; i < channel.length; i++) {
        // const b = Math.random() * 2 - 1
        const b = this.buffer[i]
        channel[i] = b * parameters.gain[0]
      }
    })
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
