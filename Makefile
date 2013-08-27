#
# Makefile for DBUtl
#
# ajd 25 Mar 2010
#
# TODO: regression tests (make check)
#


PKGNAME := DBUtl

ifndef CUDB_PREFIX
  CUDB_PREFIX := /usr/local
endif

ifeq ($(shell [ -f SharedFiles/MakefileCommon ] && echo y), y)
  include SharedFiles/MakefileCommon
else
  include $(CUDB_PREFIX)/share/DBUtl/MakefileCommon
endif

################################################################################

CXX := $(shell which c++)

################################################################################
#
# List of all targets ...
#

.PHONY : all
all: $(LIB)/libDBUtl.$(LIB_EXT)

#
################################################################################

################################################################################
#
# Special handling for /usr/CUDB/share/DBUtl ...
#

.PHONY : install_share
install: install_share

install_share:
	$(INSTALL) -d -m 755 $(datarootdir)/$(PKGNAME) \
		&& cd SharedFiles && $(INSTALL) -m 644 MakefileCommon $(datarootdir)/$(PKGNAME)

#
################################################################################

$(eval $(call GrpBEGIN,$(SRC)))

$(eval $(call GrpADD4LIB,AlignedMem CmdLine CommSocket CommUtils Log LogCPP RNG RNG2 Runnable \
	TCPSocket Thread Socket SocketException ptimer zipf))

$(eval $(call GrpMKLIB,DBUtl))

$(eval $(call GrpEND))



