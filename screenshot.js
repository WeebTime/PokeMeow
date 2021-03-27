'use strict'

const VBAEmulator = require('./pokemeowtool/vba').VBAEmulator;
const fs = require('fs');

var vba = new VBAEmulator();

vba.screenshot(function (err, buf) {
  if (err) {
    return;
  }
  fs.writeFile('./screenshot.png', buf, () => {});
});
