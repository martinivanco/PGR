#!/bin/sh

run_type=$1

if [ "$run_type" = "--make" ]; then
	cmake -H. -Bbuild
	make -C build
	build/raytracer sample_scene.txt
fi
if [ "$run_type" = "--run" ]; then
  	build/raytracer sample_scene.txt
fi
if [ "$run_type" = "--clear" ]; then
  rm -r build
fi
