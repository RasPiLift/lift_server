CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := lift
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
LIB := -ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server -lwiringPi -lpthread -lboost_thread -lboost_system
INC := -I include

$(TARGET): $(OBJECTS)
	@echo "Linking ..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Building ..."
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(shell cp -n $(SRCDIR)/example_config.xml config.xml)

clean:
	@echo "Cleaning ..."; 
	@echo " $(RM) -r $(BUILDDIR); rm $(TARGET)"; $(RM) -r $(BUILDDIR); rm $(TARGET)

.PHONY: clean
