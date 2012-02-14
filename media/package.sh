#!/bin/sh

rm -f fonts.zip
rm -f images.zip
rm -f materials.zip
rm -f meshes.zip

zip -j fonts.zip fonts/*
zip -j images.zip images/*
zip -j materials.zip materials/*
zip -j meshes.zip meshes/*
