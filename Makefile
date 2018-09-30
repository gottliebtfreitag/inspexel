TARGET = inspexel

# compiler
CROSS_COMPILE_PREFIX = 
CC      ?= $(CROSS_COMPILE_PREFIX)gcc
CXX     ?= $(CROSS_COMPILE_PREFIX)g++
LD      ?= $(CROSS_COMPILE_PREFIX)g++
SIZE    ?= $(CROSS_COMPILE_PREFIX)size
GDB     ?= $(CROSS_COMPILE_PREFIX)gdb
OBJ_CPY ?= $(CROSS_COMPILE_PREFIX)objcopy

PREFIX              ?=
INSTALL_BIN_DIR     ?= $(PREFIX)/usr/bin
BASH_COMPLETION_DIR ?= $(PREFIX)/usr/share/bash-completion/completions/
ZSH_COMPLETION_DIR  ?= $(PREFIX)/usr/share/zsh-completion/completions/
MAN_DIR             ?= $(PREFIX)/usr/share/man/man1/

SRC_FOLDERS = src/
LIBS = c pthread stdc++fs fuse
LIB_PATHS =
INCLUDES = src/ \

MAP_FILE = $(TARGET).map


INCLUDE_CMD = $(addprefix -I, $(INCLUDES))
LIB_CMD = $(addprefix -l, $(LIBS))
LIB_PATH_CMD = $(addprefix -L, $(LIB_PATHS))

# Flags

DEFINES		+= -D_FILE_OFFSET_BITS=64

FP_FLAGS       ?=
COMMON_FLAGS	+= $(DEFINES) $(FP_FLAGS)
COMMON_FLAGS	+= -O0 -g3
COMMON_FLAGS	+= $(INCLUDE_CMD)

# Warnings
W_FLAGS      += -Wextra -Wredundant-decls
W_FLAGS      += -Wall -Wundef

###############################################################################
# C flags

CFLAGS		+= $(COMMON_FLAGS)
CFLAGS		+= $(W_FLAGS)
CFLAGS      += -Wimplicit-function-declaration -Wmissing-prototypes -Wstrict-prototypes

###############################################################################
# C++ flags

CPPFLAGS	+= $(COMMON_FLAGS)
CPPFLAGS	+= $(W_FLAGS)
# add this for link-time template instanciation
CPPFLAGS	+= -std=c++17
CPPFLAGS	+= -I$(INCLUDE_DIR)


###############################################################################
# Linker flags

LINKERFLAGS +=  $(COMMON_FLAGS)
#LINKERFLAGS += -s

CPP_SUFFIX = .cpp
C_SUFFIX   = .c
OBJ_SUFFIX = .o
DEP_SUFFIX = .d
OBJ_DIR    = obj/

IGNORE_STRINGS = /archive/

CPP_FILES            += $(sort $(filter-out $(IGNORE_STRINGS), $(foreach SRC_FOLDER, $(SRC_FOLDERS), $(shell find $(SRC_FOLDER) -name "*$(CPP_SUFFIX)" | grep -v $(addprefix -e, $(IGNORE_STRINGS))))))
C_FILES              += $(sort $(filter-out $(IGNORE_STRINGS), $(foreach SRC_FOLDER, $(SRC_FOLDERS), $(shell find $(SRC_FOLDER) -name "*$(C_SUFFIX)" | grep -v $(addprefix -e, $(IGNORE_STRINGS))))))

CPP_OBJ_FILES        += $(addsuffix $(OBJ_SUFFIX), $(addprefix $(OBJ_DIR), $(CPP_FILES)))
C_OBJ_FILES          += $(addsuffix $(OBJ_SUFFIX), $(addprefix $(OBJ_DIR), $(C_FILES)))

DEP_FILES            += $(addprefix $(OBJ_DIR), $(addsuffix $(DEP_SUFFIX), $(CPP_FILES) $(C_FILES)))

ifndef VERBOSE
SILENT = @
endif


.phony: all clean flash

all: $(TARGET)

dbg:
	@ echo $(CPP_FILES)

clean:
	$(SILENT) rm -rf $(OBJ_DIR) $(TARGET) $(TARGET).map $(TARGET).bin

install: $(TARGET)
	$(SILENT) mkdir -p $(INSTALL_BIN_DIR)
	$(SILENT) cp $< $(INSTALL_BIN_DIR)
	$(SILENT) mkdir -p $(BASH_COMPLETION_DIR)
	$(SILENT) cp scripts/bash_completion $(BASH_COMPLETION_DIR)/$<
	$(SILENT) mkdir -p $(ZSH_COMPLETION_DIR)
	$(SILENT) cp scripts/zsh_completion $(ZSH_COMPLETION_DIR)/$<
	$(SILENT) mkdir -p $(MAN_DIR)
	$(SILENT) scripts/generateMan.sh | gzip > $(MAN_DIR)/$<.1.gz

$(TARGET): $(CPP_OBJ_FILES) $(C_OBJ_FILES)
	@echo linking $(TARGET)
	$(SILENT) $(CXX) -o $@ $^ $(LINKERFLAGS) $(LIB_PATH_CMD) $(LIB_CMD)
	$(SILENT) $(SIZE) $@
	@ echo done

$(OBJ_DIR)%$(C_SUFFIX)$(OBJ_SUFFIX): %$(C_SUFFIX)
	@echo building $<
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(INCLUDE_CMD) -MM -MF $(OBJ_DIR)$<.d -c $<
	@ mv -f $(OBJ_DIR)$<.d $(OBJ_DIR)$<.d.tmp
	@ sed -e 's|.*:|$@:|' < $(OBJ_DIR)$<.d.tmp > $(OBJ_DIR)$<.d
	@ rm -f $(OBJ_DIR)$<.d.tmp

	$(SILENT) $(CC) $(CFLAGS) $(INCLUDE_CMD) -o $@ -c $<


$(OBJ_DIR)%$(CPP_SUFFIX)$(OBJ_SUFFIX): %$(CPP_SUFFIX)
	@echo building $<
	@ mkdir -p $(dir $@)
	@ $(CXX) $(CPPFLAGS) $(INCLUDE_CMD) -MM -MF $(OBJ_DIR)$<.d -c $<
	@ mv -f $(OBJ_DIR)$<.d $(OBJ_DIR)$<.d.tmp
	@ sed -e 's|.*:|$@:|' < $(OBJ_DIR)$<.d.tmp > $(OBJ_DIR)$<.d
	@ rm -f $(OBJ_DIR)$<.d.tmp

	$(SILENT) $(CXX) $(CPPFLAGS) $(INCLUDE_CMD) -o $@ -c $<

-include $(DEP_FILES)
