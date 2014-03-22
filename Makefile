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
DESTDIR = /
endif

install: all check_destdir
	strip bin/wildbar
	cp bin/wildbar "$(DESTDIR)"/usr/bin/wildbar
	mkdir -p "$(DESTDIR)"/etc/wildbar
	cp wildbar.conf "$(DESTDIR)"/etc/wildbar/wildbar.conf

uninstall: check_destdir
	rm -f "$(DESTDIR)"/bin/wildbar
	rm -f "$(DESTDIR)"/etc/wildbar/wildbar.conf
