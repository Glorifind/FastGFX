#!/usr/bin/env bash

mkdir -p deps
cd deps
  git clone https://github.com/g-truc/glm
  cd glm
    git checkout 0.9.7.5
  cd ..
cd ..
