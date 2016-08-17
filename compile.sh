#!/usr/bin/env bash

cmake .
make

cd js
  npm install
  node build-js.js
cd ..
