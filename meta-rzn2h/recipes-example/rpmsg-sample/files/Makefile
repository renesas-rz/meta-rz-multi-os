PROGRAM = rpmsg_sample_client
CFLAGS = -Wall -O2 -g -DCFG_CA5X $(EXTRA_CFLAGS)
LINK_LIBS = -lopen_amp -lmetal

OBJS += main.o
OBJS += helper.o
OBJS += rzn2_rproc.o
OBJS += platform_info.o

.SUFFIXES: .c .o

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGRAM) $^ $(LINK_LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
