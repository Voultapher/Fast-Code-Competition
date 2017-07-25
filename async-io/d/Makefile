DMD=bin/dmd2/linux/bin64/dmd
DMD_VERSION=2.074.0
LDC=bin/ldc2-$(LDC_VERSION)-linux-$(PLATFORM)/bin/ldc2
LDC_VERSION=1.3.0
DFLAGS=-g
PLATFORM=x86_64

all: bin/async_opt bin/serial_opt

################################################################################
# Auto-bootstrap DMD & LDC for outdated Debian/Ubuntu
################################################################################

bin:
	mkdir -p $@

bin/dmd2: | bin
	curl -fSL --retry 3 "http://downloads.dlang.org/releases/2.x/$(DMD_VERSION)/dmd.$(DMD_VERSION).linux.tar.xz" | tar -Jxf - -C $|
bin/dmd2/linux/bin64/dmd: | bin/dmd2

bin/ldc2-$(LDC_VERSION)-linux-$(PLATFORM): | bin
	curl -fSL --retry 3 "https://github.com/ldc-developers/ldc/releases/download/v$(LDC_VERSION)/ldc2-$(LDC_VERSION)-linux-$(PLATFORM).tar.xz" \
	| tar -Jxf - -C $|

bin/ldc2-$(LDC_VERSION)-linux-$(PLATFORM)/bin/ldc2: | bin/ldc2-$(LDC_VERSION)-linux-$(PLATFORM)


################################################################################
# Programs
################################################################################


bin/%_opt: %.d $(LDC) | bin
	$(LDC) -g -O4 -mcpu=native -release $(DFLAGS) $< -of$@

bin/%: %.d $(DMD) | bin
	$(DMD) $(DFLAGS) $< -of$@

benchmark: bin/serial_opt bin/async_opt
	../benchmark.sh $< $(word 2, $^)

benchmark2: bin/serial_opt bin/async_putchar_opt
	../benchmark.sh $< $(word 2, $^)

test: bin/async_putchar_opt
	../test.sh $<
