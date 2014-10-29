# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

PREFIX = /usr
BIN = /bin
BINDIR = $(PREFIX)$(BIN)
DATA = /share
DEVDIR = /dev
SYSCONFDIR = /etc
LOGDIR = $(DEVDIR)/log
DATADIR = $(PREFIX)$(DATA)
LICENSES = $(DATADIR)/licenses

PKGNAME = cerberus-logging


OPTIMISE = -Os
STD = -std=gnu99
WARN = -Wall -Wextra -Wdouble-promotion -Wformat=2 -Winit-self -Wmissing-include-dirs            \
       -Wtrampolines -Wfloat-equal -Wshadow -Wmissing-prototypes -Wmissing-declarations          \
       -Wredundant-decls -Wnested-externs -Winline -Wno-variadic-macros -Wsync-nand              \
       -Wunsafe-loop-optimizations -Wcast-align -Wstrict-overflow -Wdeclaration-after-statement  \
       -Wundef -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wlogical-op -Waggregate-return   \
       -Wstrict-prototypes -Wold-style-definition -Wpacked -Wvector-operation-performance        \
       -Wunsuffixed-float-constants -Wsuggest-attribute=const -Wsuggest-attribute=noreturn       \
       -Wsuggest-attribute=pure -Wsuggest-attribute=format -Wnormalized=nfkc -Wconversion        \
       -fstrict-aliasing -fstrict-overflow -fipa-pure-const -ftree-vrp -fstack-usage             \
       -funsafe-loop-optimizations -pedantic
DEFS = -D'LOGDIR="$(LOGDIR)"'

SYSTEMS = btmp utmp lastlog syslog audit

L_btmp =
L_utmp =
L_lastlog =
L_syslog =
L_audit = -laudit


.PHONY: all
all: $(SYSTEMS)

.PHONY: btmp utmp lastlog syslog audit
btmp: bin/log-login-btmp
utmp: bin/log-login-utmp
lastlog: bin/log-login-lastlog
syslog: bin/log-login-syslog
audit: bin/log-login-audit

bin/log-login-%: obj/log-login-%.o obj/common.o
	@mkdir -p bin
	$(CC) $(STD) $(OPTIMISE) $(WARN) $(LDFLAGS) $(L_$*) -o $@ $^

obj/%.o: src/%.c src/*.h
	@mkdir -p obj
	$(CC) $(STD) $(OPTIMISE) $(WARN) $(DEFS) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<


.PHONY: clean
clean:
	-rm -r bin obj *.su src/*.su

