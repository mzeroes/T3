#!/usr/bin/env  bash

# Make sure we're in the project root.
cd $(dirname "$0")/..

cd build

cmake .. -DCMAKE_BUILD_TYPE=DEBUG

make
