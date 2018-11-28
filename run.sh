#!/bin/sh

cmake -H. -Bbuild
make -C build
build/raytracer sample_scene.txt