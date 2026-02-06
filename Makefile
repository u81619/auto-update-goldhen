# Project Name
TITLE       := Auto Update GoldHEN
TITLE_ID    := AUG000001
VERSION     := 01.00
CONTENT_ID  := EP9000-AUG000001_00-AUTOUPDATEGH0001
APP_TYPE    := 0x00010000 # Application Type (e.g., Game)
APP_VER     := 01.00

# Toolchain
OO_PS4_TOOLCHAIN := $(ORBISDEV)/usr
CC      := $(OO_PS4_TOOLCHAIN)/bin/orbis-clang
CXX     := $(OO_PS4_TOOLCHAIN)/bin/orbis-clang++
AS      := $(OO_PS4_TOOLCHAIN)/bin/orbis-as
AR      := $(OO_PS4_TOOLCHAIN)/bin/orbis-ar
LD      := $(OO_PS4_TOOLCHAIN)/bin/orbis-ld
STRIP   := $(OO_PS4_TOOLCHAIN)/bin/orbis-strip
MKSFOEX := $(OO_PS4_TOOLCHAIN)/bin/mksfoex
ORBIS_PUB_CMD := $(OO_PS4_TOOLCHAIN)/bin/orbis-pub-cmd

# Paths
PROJDIR := .
INCDIR  := $(PROJDIR)/include
SRCDIR  := $(PROJDIR)/source
ASSETS  := $(PROJDIR)/assets
SCE_SYS := $(PROJDIR)/sce_sys
BUILDDIR := build

# Compilation Flags
CFLAGS   := -I$(INCDIR) -O2 -Wall
CXXFLAGS := $(CFLAGS) -std=c++11
LDFLAGS  := -L$(OO_PS4_TOOLCHAIN)/lib -lSceKernel -lSceSystemMsgDialog -lSceCommonDialog -lSceSysmodule

# Files
SRCS    := $(wildcard $(SRCDIR)/*.cpp)
OBJS    := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))
TARGET  := eboot.bin

# Rules
all: $(TARGET) pkg

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(BUILDDIR)/$(TARGET).elf
	$(STRIP) --strip-unneeded $(BUILDDIR)/$(TARGET).elf
	# Convert ELF to eboot.bin (requires create-eboot from SDK)
	$(OO_PS4_TOOLCHAIN)/bin/create-eboot $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/$(TARGET)

pkg: $(TARGET) $(SCE_SYS)/param.sfo $(SCE_SYS)/icon0.png $(SCE_SYS)/pic0.png
	@echo "Packaging into PKG..."
	@mkdir -p $(BUILDDIR)/pkg_temp/sce_sys
	@cp $(BUILDDIR)/$(TARGET) $(BUILDDIR)/pkg_temp/eboot.bin
	@cp $(SCE_SYS)/param.sfo $(BUILDDIR)/pkg_temp/sce_sys/param.sfo
	@cp $(SCE_SYS)/icon0.png $(BUILDDIR)/pkg_temp/sce_sys/icon0.png
	@cp $(SCE_SYS)/pic0.png $(BUILDDIR)/pkg_temp/sce_sys/pic0.png
	@mkdir -p $(BUILDDIR)/pkg_temp/assets
	@cp $(ASSETS)/payload.bin $(BUILDDIR)/pkg_temp/assets/payload.bin
	@cp $(ASSETS)/goldhen.bin $(BUILDDIR)/pkg_temp/assets/goldhen.bin
	$(ORBIS_PUB_CMD) img_create --no_progress_bar --content_id $(CONTENT_ID) $(BUILDDIR)/pkg_temp $(TITLE_ID).pkg

$(SCE_SYS)/param.sfo:
	@echo "Creating param.sfo..."
	@mkdir -p $(SCE_SYS)
	$(MKSFOEX) -v "$(VERSION)" -i "$(TITLE_ID)" -t "$(TITLE)" -a "$(APP_TYPE)" -y "$(APP_VER)" $@

$(SCE_SYS)/icon0.png:
	@echo "Copying icon0.png..."
	@cp $(PROJDIR)/sce_sys/icon0.png $@

$(SCE_SYS)/pic0.png:
	@echo "Copying pic0.png..."
	@cp $(PROJDIR)/sce_sys/pic0.png $@

clean:
	rm -rf $(BUILDDIR) $(TARGET) $(SCE_SYS)/param.sfo $(TITLE_ID).pkg
