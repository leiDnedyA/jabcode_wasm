#!/bin/bash

TARGET="./build/libjabcode.a"
TARGET_JS="./build/libjabcode.js"

make clean
# emmake make
# emcc js_binds.c -sSINGLE_FILE -sEXPORTED_FUNCTIONS=get_default_symbol_number -sEXPORTED_RUNTIME_METHODS=ccall,cwrap -o "$TARGET_JS"
emcc js_binds.c -sSINGLE_FILE -sMODULARIZE -sEXPORTED_RUNTIME_METHODS=ccall -o "$TARGET_JS"
echo "export default Module;" >> "$TARGET_JS"

