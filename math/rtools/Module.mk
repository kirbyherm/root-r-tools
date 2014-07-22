# Module.mk for rtools module
# Copyright (c) 2014 Kirby Hermansen and Lorenzo Moneta
#
# Author: Kirby Hermansen, 22/07/2014

MODNAME      := rtools
MODDIR       := $(ROOT_SRCDIR)/math/$(MODNAME)
MODDIRS      := $(MODDIR)/src
MODDIRI      := $(MODDIR)/inc

RTOOLSDIR    := $(MODDIR)
RTOOLSDIRS   := $(RTOOLSDIR)/src
RTOOLSDIRI   := $(RTOOLSDIR)/inc

##### libRtools #####
RTOOLSL      := $(MODDIRI)/LinkDef.h
RTOOLSDS     := $(call stripsrc,$(MODDIRS)/G__Rtools.cxx)
RTOOLSDO     := $(RTOOLSDS:.cxx=.o)
RTOOLSDH     := $(RTOOLSDS:.cxx=.h)

RTOOLSH      := $(filter-out $(MODDIRI)/LinkDef%,$(wildcard $(MODDIRI)/*.h))
RTOOLSS      := $(filter-out $(MODDIRS)/G__%,$(wildcard $(MODDIRS)/*.cxx))
RTOOLSO      := $(call stripsrc,$(RTOOLSS:.cxx=.o))

RTOOLSDEP    := $(RTOOLSO:.o=.d) $(RTOOLSDO:.o=.d)

RTOOLSLIB    := $(LPATH)/libRtools.$(SOEXT)
RTOOLSMAP    := $(RTOOLSLIB:.$(SOEXT)=.rootmap)

# used in the main Makefile
ALLHDRS     += $(patsubst $(MODDIRI)/%.h,include/%.h,$(RTOOLSH))
ALLLIBS     += $(RTOOLSLIB)
ALLMAPS     += $(RTOOLSMAP)

# include all dependency files
INCLUDEFILES += $(RTOOLSDEP)

##### local rules #####
.PHONY:         all-$(MODNAME) clean-$(MODNAME) distclean-$(MODNAME)

include/%.h:    $(RTOOLSDIRI)/%.h
		cp $< $@

$(RTOOLSLIB):   $(RTOOLSO) $(RTOOLSDO) $(ORDER_) $(MAINLIBS) $(RTOOLSLIBDEP)
		@$(MAKELIB) $(PLATFORM) $(LD) "$(LDFLAGS)" \
		   "$(SOFLAGS)" libRtools.$(SOEXT) $@ "$(RTOOLSO) $(RTOOLSDO)" \
		   "$(RTOOLSLIBEXTRA)"

$(RTOOLSDS):    $(RTOOLSH) $(RTOOLSL) $(ROOTCINTTMPDEP)
		$(MAKEDIR)
		@echo "Generating dictionary $@..."
		$(ROOTCINTTMP) -f $@ -c $(RTOOLSH) $(RTOOLSL)

$(RTOOLSMAP):   $(RLIBMAP) $(MAKEFILEDEP) $(RTOOLSL)
		$(RLIBMAP) -o $@ -l $(RTOOLSLIB) \
		   -d $(RTOOLSLIBDEPM) -c $(RTOOLSL)

all-$(MODNAME): $(RTOOLSLIB) $(RTOOLSMAP)

clean-$(MODNAME):
		@rm -f $(RTOOLSO) $(RTOOLSDO)

clean::         clean-$(MODNAME)

distclean-$(MODNAME): clean-$(MODNAME)
		@rm -f $(RTOOLSDEP) $(RTOOLSDS) $(RTOOLSDH) $(RTOOLSLIB) $(RTOOLSMAP)

distclean::     distclean-$(MODNAME)

# Optimize dictionary with stl containers.
$(RTOOLSDO): NOOPT = $(OPT)
