module.exports = global.process ? global.process.hrtime.bind(process) : null
