const sampler = require('./')()

setInterval(function () {
  if (sampler.delay) console.log(sampler)
}, 1000)

setInterval(function () {
  for (let i = 0; i < 1e7; i++);
}, 3000)
