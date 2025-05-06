#!/usr/bin/env bash

BASEDIR=$(dirname "$0")

cp -r $BASEDIR/dayN/ $BASEDIR/day$1/
mv  $BASEDIR/day$1/dayN.cpp $BASEDIR/day$1/day$1.cpp