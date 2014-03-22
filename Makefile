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

install: all
	install -s bin/wildbar /bin/wildbar; \
  install wildbar.conf /etc/wildbar.conf; \

uninstall:
	rm /bin/wildbar; \
	rm /etc/wildbar.conf; \
