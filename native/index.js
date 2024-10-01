const binding = require('node-gyp-build')(__dirname)

module.exports = function samplerNative () {
  const buf = Buffer.alloc(binding.sizeof_delay_timer_t)
  const view = new Uint32Array(buf.buffer, buf.byteOffset)

  binding.start_delay_timer(buf)

  return {
    native: true,
    get delay () {
      return view[0]
    },
    get times () {
      return view[1]
    },
    destroy () {
      binding.stop_delay_timer(buf)
    },
    [Symbol.for('bare.inspect')] () {
      return { native: true, delay: this.delay, times: this.times, destroy: this.destroy }
    },
    [Symbol.for('nodejs.util.inspect.custom')] () {
      return [Symbol.for('bare.inspect')]()
    }
  }
}
