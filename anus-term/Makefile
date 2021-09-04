#The Target Binary Program
TARGET        := anus-term

#Compiler
CC            := g++

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR        := src
INCDIR        := include
BUILDDIR      := obj
TARGETDIR     := bin
DOCDIR        := doc

#Flags, Libraries and Includes
CXXFLAGS       += -Wall -Wextra -std=c++1z -pedantic -Wshadow -fdiagnostics-color=auto -I $(INCDIR)
CXXFLAGS_DEBUG += -g -Og -fsanitize=address -D_GLIBCXX_DEBUG -fsanitize=undefined
CXXFLAGS_PROD  += -O2 -DNDEBUG -flto
CPPFLAGS       +=
LDFLAGS        += -s -flto
LDLIBS         += -lutil -fsanitize=address -fsanitize=undefined


#all:
#g++ -std=c++1z -lutil term.cpp
#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
DEBUG         ?= 1

ifeq ($(DEBUG), 1)
	CXXFLAGS  += $(CXXFLAGS_DEBUG)
	BUILDDIR  := $(BUILDDIR)/debug
	TARGET    := $(TARGET)
else
	CXXFLAGS  += $(CXXFLAGS_PROD)
	BUILDDIR  := $(BUILDDIR)/prod
endif

SOURCES       := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS       := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

all: $(TARGETDIR)/$(TARGET) ## Build the terminal

run: $(TARGETDIR)/$(TARGET) ## Build and run the terminal
	$(TARGETDIR)/$(TARGET)

-include $(OBJECTS:.o=.d)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	@echo "  CC     $@"
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(TARGETDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	@echo "Linking  $@"
	$(LINK.o) $(OUTPUT_OPTION) $^ $(LDLIBS)

clean: ## Clean the project
	$(RM) -r obj $(TARGETDIR) $(DOCDIR)

help: ## Prints help for targets with comments
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) \
		| awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

check: all ## Run check (not implemented yet)

.PHONY: all clean check doc help
