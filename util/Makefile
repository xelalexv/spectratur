
#
# needed packages:
#
#	build-essential
#	libx11-dev
#	libxmu-dev
#	libgtk-3-dev
#

kev: kev.c log.c log.h
	gcc kev.c log.c -o kev -Wall -lX11 -lXmu -DLOG_USE_COLOR \
		$(shell pkg-config --cflags --libs gtk+-3.0)

.PHONY: clean
clean:
	rm kev
