CC=gcc
INC1 = 
INC2 = 
INCDIRS = -I${INC1} -I${INC2}
LIBDIR1 = 
LIBDIR2 = 
#CFLAGS=-g -Wall ${INCDIRS}	
CFLAGS=-g  ${INCDIRS}	
LDFLAGS = -lwiringPi 

SOURCES=mcp3008.c
OBJECTS=$(SOURCES:.c=.o)

SOURCES1=mcp3008_2.c
OBJECTS1=$(SOURCES1:.c=.o)

EXECUTABLE=mcp3008
EXECUTABLE1=mcp3008_2

all:$(EXECUTABLE) $(EXECUTABLE1)
$(EXECUTABLE):$(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@   
$(EXECUTABLE1):$(OBJECTS1) 
	$(CC) $(CFLAGS) $(OBJECTS1) $(LDFLAGS) -o $@   
clean:
	rm -f $(OBJECTS) $(OBJECTS1) $(EXECUTABLE) $(EXECUTABLE1)