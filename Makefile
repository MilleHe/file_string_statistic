default: build


###############################################################################
OBJDIR     = $(PWD)/obj
INCDIR     = $(PWD)/src
SRCDIR     = $(PWD)/src
EXEDIR     = $(PWD)/bin


###############################################################################
CC         = g++
CFLAGS     = -Werror -Wall -fPIC -std=c++17
CPPFLAGS   = -D_REENTRANT -D_GNU_SOURCE

dev_OPT    = -g3
build_OPT  = -O2 -g

###############################################################################
OBJS += \
  $(OBJDIR)/main.o


###############################################################################
#$(OBJDIR)/file_statistic.o: $(SRCDIR)/file_statistic.cpp \
#  $(INCDIR)/file_statistic.h $(INCDIR)/common.h
#	$(CC) $(CFLAGS) $(CPPFLAGS) $(OPT) $< -c -o $@

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp $(INCDIR)/common.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OPT) $< -c -o $@
	
	
###############################################################################
$(EXEDIR)/main: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)


###############################################################################
.PHONY: dev build clean dirs

build: dirs
	$(MAKE) $(OBJS) \
          CFLAGS="$(CFLAGS)" OPT="$(build_OPT)" CPPFLAGS="$(CPPFLAGS)"
	$(MAKE) $(EXEDIR)/main OBJS="$(OBJS)" \
          LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)"

dev: dirs
	$(MAKE) $(OBJS) \
	  CFLAGS="$(CFLAGS)" OPT="$(dev_OPT)" CPPFLAGS="$(CPPFLAGS)"
	$(MAKE) $(EXEDIR)/main OBJS="$(OBJS)" \
	  LIBS="$(LIBS)"


##############################################################################
clean:
	@startdir=$(PWD)
	rm -rf $(OBJDIR)*
	rm -rf $(EXEDIR)*


##############################################################################
dirs:
	mkdir -p $(OBJDIR) $(EXEDIR)

