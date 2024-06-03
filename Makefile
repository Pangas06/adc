
# Switch Makefile

MKDIR = mkdir
ifeq ($(OS),Windows_NT)
    uname_S := Windows

	LN = cp
else
    uname_S := $(shell uname -s)

	LN = ln -sf
endif
CP = cp

default: sdcc

help:
	@echo "# Vyber si toolchain!"
	@echo "# Detaily v REAME a na adrese "
	@echo "# https://chytrosti.marrek.cz/stm8oss.html"
	@echo
	@echo "# Pokud chces debug a nevadi ti vetsi binarka:"
	@echo \"make sdcc\" "     ... nebo"
	@echo \"make default\"
	@echo
	@echo "# Pokud chces male binarky a nepotrebujes debug:"
	@echo \"make sdccrm\"
	@echo
	@echo "# Pokud chces male binarky i debug a mas 'SDCC-gas' zavolej:"
	@echo \"make sdcc-gas\"


sdcc: spl
	$(LN) .make/Makefile-sdcc Makefile || cp .make/Makefile-sdcc Makefile
	@echo "# znovu zavolej make"
sdccrm: spl
	$(LN) .make/Makefile-sdccrm Makefile || cp .make/Makefile-sdccrm Makefile
	@echo "# znovu zavolej make"
sdcc-gas: spl
	$(LN) .make/Makefile-sdcc-gas Makefile || cp .make/Makefile-sdcc-gas Makefile
	@echo "# znovu zavolej make"

spl:
	bash .make/spl.sh
