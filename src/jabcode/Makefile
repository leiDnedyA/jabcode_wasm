PREFIX 	=
CC 	= $(PREFIX)gcc
AR 	= $(PREFIX)ar
RANLIB	= $(PREFIX)ranlib
CFLAGS	= -O2 -std=c11

TARGET = build/libjabcode.a
TARGET_JS = build/libjabcode.js

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

$(TARGET): $(OBJECTS)
	$(AR) cru $@ $?
	$(RANLIB) $@

$(OBJECTS): %.o: %.c
	$(CC) -c -I. -I./include $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(TARGET_JS) $(OBJECTS)
