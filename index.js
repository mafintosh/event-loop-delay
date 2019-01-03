try {
  module.exports = require('./native')
} catch (_) {
  module.exports = require('./fallback')
}
