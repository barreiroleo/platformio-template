# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

.PHONY: all gen build build-release test
all: gen build test

gen:
	pio pkg install
	pio run --target compiledb

build:
	pio run -e atmega328p_debug

build-release:
	pio run -e atmega328p_release

test:
	stdbuf -o0 pio test --without-uploading


.PHONY: upload clean program uploadfs update
upload:
	pio run --target upload

clean:
	pio run --target clean

program:
	pio run --target program

uploadfs:
	pio run --target uploadfs

update:
	pio pkg update
