const isBare = !!global.Bare
const hrtime = isBare ? require('bare-hrtime') : global.process.hrtime

module.exports = function samplerJavascript () {
  let prev = now()

  const interval = setInterval(tick, 10)
  interval.unref()

  const delayer = {
    native: false,
    delay: 0,
    times: 0,
    destroy () {
      clearInterval(interval)
    }
  }

  return delayer

  function tick () {
    const time = now()
    const delta = time - prev

    prev = time

    if (delta > 15) {
      delayer.times++
      delayer.delay += delta - 10
    }
  }
}

function now () {
  const ts = hrtime()
  return (ts[0] * 1e3) + Math.floor((ts[1] / 1e6))
}
