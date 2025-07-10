PREFIX 	=
CC 	= $(PREFIX)emcc
AR 	= $(PREFIX)emar
RANLIB	= $(PREFIX)emranlib
CFLAGS	= -O2 -std=c11
EMCCFLAGS = -s USE_LIBPNG=1 -s SINGLE_FILE -s FORCE_FILESYSTEM=1 -s MAXIMUM_MEMORY=256MB -s EXPORTED_RUNTIME_METHODS='["FS","stringToNewUTF8","UTF8ToString"]' -s EXPORTED_FUNCTIONS='["_free","_malloc"]'

TARGET = build/libjabcode.a
TARGET_JS = build/libjabcode.js
JS_BINDS = js_binds

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all
all: $(TARGET) $(TARGET_JS)

$(TARGET): $(OBJECTS)
	$(AR) cru $@ $?
	$(RANLIB) $@

$(OBJECTS): %.o: %.c
	$(CC) -c -I. -I./include $(CFLAGS) $< -o $@


$(TARGET_JS): $(TARGET) $(JS_BINDS).o
	@echo 'building target_js'
	$(CC) $(JS_BINDS).o -Lbuild -ljabcode -o $(TARGET_JS) $(EMCCFLAGS)
	echo "export default Module;" >> $(TARGET_JS)

force_js:

clean:
	rm -f $(TARGET) $(TARGET_JS) $(OBJECTS)
