all:
	gcc dwmblocks.c -lX11 -g -o dwmblocks

install: all
	cp dwmblocks /usr/local/bin/

uninstall:
	rm /usr/local/bin/dwmblocks
