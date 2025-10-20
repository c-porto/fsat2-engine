APP = fsat2-engine

APP_OBJS = src/fsat2-engine.o

LDLIBS += -lzmq

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) -I./ -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)
clean:
	rm -f $(APP) *.o


