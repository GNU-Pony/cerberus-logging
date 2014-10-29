# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

PREFIX = /usr
BIN = /bin
BINDIR = $(PREFIX)$(BIN)
DATA = /share
DEVDIR = /dev
LOGDIR = $(DEVDIR)/log
DATADIR = $(PREFIX)$(DATA)
LICENSEDIR = $(DATADIR)/licenses

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
DEFAULT_SYSTEMS = utmp

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


.PHONY: install
install: install-base install-default-systems

.PHONY: install-all
install-all: install-base install-systems

.PHONY: install-base
install-base: install-logging install-license

.PHONY: install-logging
install-logging:
	install -dm755 -- "$(DESTDIR)$(DATADIR)/$(PKGNAME)"
	install -m755 -- src/logging "$(DESTDIR)$(DATADIR)/$(PKGNAME)/logging"

.PHONY: install-license
install-license:
	install -dm755 -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"
	install -m644 -- COPYING LICENSE "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"

.PHONY: install-systems
install-systems: $(foreach S,$(SYSTEMS),install-$(S))

.PHONY: install-default-systems
install-default-systems: $(foreach S,$(DEFAULT_SYSTEMS),install-$(S))

.PHONY: install-btmp
install-btmp: bin/log-login-btmp
	install -dm755 -- "$(DESTDIR)$(BINDIR)"
	install $< -- "$(DESTDIR)$(BINDIR)/log-login-btmp"

.PHONY: install-utmp
install-utmp: bin/log-login-utmp
	install -dm755 -- "$(DESTDIR)$(BINDIR)"
	install $< -- "$(DESTDIR)$(BINDIR)/log-login-utmp"

.PHONY: install-lastlog
install-lastlog: bin/log-login-lastlog
	install -dm755 -- "$(DESTDIR)$(BINDIR)"
	install $< -- "$(DESTDIR)$(BINDIR)/log-login-lastlog"

.PHONY: install-syslog
install-syslog: bin/log-login-syslog
	install -dm755 -- "$(DESTDIR)$(BINDIR)"
	install $< -- "$(DESTDIR)$(BINDIR)/log-login-syslog"

.PHONY: install-audit
install-audit: bin/log-login-audit
	install -dm755 -- "$(DESTDIR)$(BINDIR)"
	install $< -- "$(DESTDIR)$(BINDIR)/log-login-audit"


.PHONY: uninstall
uninstall:
	-rm -- "$(DESTDIR)$(DATADIR)/$(PKGNAME)/logging"
	-rmdir -- "$(DESTDIR)$(DATADIR)/$(PKGNAME)"
	-rm -- $(foreach S,$(SYSTEMS),"$(DESTDIR)$(BINDIR)/log-login-$(S)")
	-rm -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)/COPYING"
	-rm -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)/LICENSE"
	-rmdir -- "$(DESTDIR)$(LICENSEDIR)/$(PKGNAME)"


.PHONY: clean
clean:
	-rm -r bin obj *.su src/*.su

