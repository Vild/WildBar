.PHONY: Ninja cleanNinja all clean check_target files run run-n

DefaultTarget = WildBar

Ninja: all
cleanNinja: clean

all: check_target files
	ninja

clean:
	ninja -t clean -g

run: all
	bin/${TARGET}

run-n: all
	bin/${TARGET} -n

check_target:
ifndef TARGET
TARGET = ${DefaultTarget}
endif

files:
	chmod +x files.sh ; TARGET=${TARGET} ./files.sh > files.ninja
