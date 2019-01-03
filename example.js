const sampler = require('./')()

setInterval(function () {
  console.log(sampler)
}, 1000)

setInterval(function () {
  for (let i = 0; i < 1e7; i++);
}, 3000)
