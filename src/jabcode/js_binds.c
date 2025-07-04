#include <emscripten.h>
#include "./include/jabcode.h"

EMSCRIPTEN_KEEPALIVE
int get_default_symbol_number() {
  return DEFAULT_SYMBOL_NUMBER;
}
