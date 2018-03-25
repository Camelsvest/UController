
# define sources root directory before everything
#SRC_ROOT := ../..

# change compiler and linker option before you include rule.mk
#
# link to libscript.a when you need to fetch configuration
# from test_script
#
DEFS := -DLINUX_DEBUG
CC := arm-linux-gnueabi-g++

CPPFLAGS := $(CPPFLAGS) -I./ -I ../

LDFLAGS := -lpthread -lstdc++ ./lib/libsupc++.a 

CFLAGS := -g -static $(DEFS)
CFLAGS := -Wall -O3 -Os -pipe \
	-mtune=cortex-a8 -march=armv7-a -mabi=aapcs-linux -mfloat-abi=softfp -mfpu=neon \
	 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
	-D_GNU_SOURCE -D_REENTRANT \
	$(CFLAGS)

COMPILE = @$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(CPPFLAGS) $(CFLAGS)

COMPILEX = @$(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
		$(CPPFLAGS) $(CFLAGS) -c $< -o $@

COMPILE_MSG = @echo "  CC\t$<"

LDFLAGS := $(LDFLAGS)

LINK = @$(CC) $(CFLAGS) $(LDFLAGS)

LINKX = @$(CC) -o ./$@ $^ $(CFLAGS) $(LDFLAGS)

LINK_MSG = @echo "  LN\t$^ -> $@"

AR = @arm-linux-gnueabi-ar

ARFLAGS := -rc

# define objects collection variable
uarttest_objs = uarttest.o ../serial_port_service.o ../hrbbus.o ../debug_log.o ../controller.o

# add your target(s) to all
.PHONY: all
all: uarttest

# define you target, the target will be output to dragonboard/output/bin
# directory
uarttest: $(uarttest_objs)
	$(LINK_MSG)
	$(LINKX)

# change 'example_objs' to your objects collection variable
$(uarttest_objs): %.o: %.cpp
	$(COMPILE_MSG)
	$(COMPILEX)

.PHONY: clean
clean:
	@echo "  CLEAN *.o"
	@rm ../*.o
	@rm *.o
	@rm uarttest
