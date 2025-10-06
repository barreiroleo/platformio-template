MAKEFLAGS += --silent
# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

.PHONY: all gen build build-release test
all: gen build test

gen:
	pio pkg install
	./generate_compiledb.py

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


.PHONY: monitor-virtual-serial coverage
monitor-virtual-serial:
	tmux new-window -n socat-pio \
	  "socat -d -d pty,raw,echo=0,link=/tmp/ttyS1 pty,raw,echo=0,link=/tmp/ttyS2" \;
	tmux split-window -v -p 93 -t socat-pio \
	  "sleep 1; pio device monitor --port /tmp/ttyS1 --baud 9600 --echo --filter time" \;
	tmux select-pane -U -t socat-pio

coverage:
	rm -rf .coverage/
	mkdir -p .coverage/
	gcovr --filter "src/" --filter "lib/" --decision --calls \
		--html-block-ids --html-theme github.dark-green --html-details .coverage/index.html \
		--json-pretty --json .coverage/index.json
	echo "HTML coverage report generated in ${PWD}/.coverage/index.html"
	echo "JSON coverage report generated in ${PWD}/.coverage/index.json"
