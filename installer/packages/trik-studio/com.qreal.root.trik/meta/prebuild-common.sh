#!/bin/bash
set -o nounset
set -o errexit

cd "$(dirname "$0")"


mkdir -p $PWD/../data/plugins/tools/kitPlugins
mkdir -p $PWD/../data/examples
cp -r  $BIN_DIR/examples/trik                                             $PWD/../data/examples
cp     $BIN_DIR/trik-runtime.tar.xz                                       $PWD/../data/
