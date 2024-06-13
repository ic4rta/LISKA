CC = gcc
CFLAGS = -Wall -Wextra
LIBRARY_NAME = libliska.a
SOURCE_DIR = src
BUILD_DIR = build
INCLUDE_DIR = $(SOURCE_DIR)
INSTALL_DIR_LIB = /usr/local/lib
INSTALL_DIR_INCLUDE = /usr/local/include

LIBRARY_SOURCES = $(SOURCE_DIR)/liska.c
LIBRARY_OBJECTS = $(BUILD_DIR)/liska.o

$(LIBRARY_NAME): $(LIBRARY_OBJECTS)
	ar rcs $@ $<

$(BUILD_DIR)/liska.o: $(LIBRARY_SOURCES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

%: %.c $(LIBRARY_NAME)
	$(CC) $(CFLAGS) $< -o $@ -I$(INCLUDE_DIR) -L$(INSTALL_DIR_LIB) -lliska

install: $(LIBRARY_NAME)
	cp $(LIBRARY_NAME) $(INSTALL_DIR_LIB)
	cp $(SOURCE_DIR)/liska.h $(INSTALL_DIR_INCLUDE)

uninstall:
	rm -f $(INSTALL_DIR_LIB)/$(LIBRARY_NAME)
	rm -f $(INSTALL_DIR_INCLUDE)/liska.h

clean:
	rm -rf $(BUILD_DIR) $(LIBRARY_NAME) main
