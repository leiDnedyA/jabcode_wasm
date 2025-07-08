PREFIX 	=
CC 	= $(PREFIX)emcc
AR 	= $(PREFIX)emar
RANLIB	= $(PREFIX)emranlib
CFLAGS	= -O2 -std=c11

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
	$(CC) $(JS_BINDS).o -Lbuild -ljabcode -o $(TARGET_JS) -s USE_LIBPNG=1 -s SINGLE_FILE -s EXPORTED_RUNTIME_METHODS='["FS"]'
	echo "export default Module;" >> $(TARGET_JS)

force_js:

clean:
	rm -f $(TARGET) $(TARGET_JS) $(OBJECTS)
