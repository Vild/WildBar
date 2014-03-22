.PHONY: Ninja cleanNinja all clean files run install uninstall

Ninja: all
cleanNinja: clean

all: files
	ninja

clean:
	ninja -t clean -g

run: all
	bin/wildbar

files:
	chmod +x files.sh ; TARGET=wildbar ./files.sh > files.ninja

check_destdir:
ifndef DESTDIR
TARGET = /
endif

install: all check_destdir
	install -s bin/wildbar ${DESTDIR}/usr/bin/wildbar; \
  install wildbar.conf ${DESTDIR}/etc/wildbar.conf; \

uninstall: check_destdir
	rm ${DESTDIR}/bin/wildbar; \
	rm ${DESTDIR}/etc/wildbar.conf
