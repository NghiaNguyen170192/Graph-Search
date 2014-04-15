CXXFLAGS = -Wall -g

EXEC = main

OBJS = Character.o Graph.o Node.o Route.o

.PHONY: all
all: $(EXEC)

$(EXEC): $(EXEC).cpp $(OBJS) -lcurl -ljson_linux-gcc-4.6_libmt

Graph.o: Graph.h Character.o Node.o Route.o
Character.o: Character.h
Node.o: Node.h
Route.o: Route.h

.PHONY: clean
clean:
	$(RM) *.o *~ core a.out $(EXEC)

