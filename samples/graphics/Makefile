# *** Automatically Generated *** DO NOT EDIT ***
# * Generated Date: 2024-09-26

include /.build-config.make

TEXTURES_DIR := $(CONFIG_APP_ASSETS_DIR)/textures
RENDER_DIR := $(CONFIG_APP_ASSETS_DIR)/render
SRC_GEN_DIR := $(CONFIG_APP_SRC_DIR)/generated

DEPS_DIR := .deps

texmake = mangl-make-tex

textures :=tex_gui

all: $(fonts) $(textures)

.PHONY: all $(fonts) $(textures)


-include $(DEPS_DIR)/tex_gui_desc.yaml.d

$(DEPS_DIR): ; @mkdir -p $@

Makefile: chessdash-graphics.yaml
	zx genbuild.mjs

#
# Generating Textures
#

tex_gui: $(TEXTURES_DIR)/tex_gui.yaml $(TEXTURES_DIR)/tex_gui.png $(SRC_GEN_DIR)/tex_gui.hpp $(TEXTURES_DIR)/tex_gui-2x.yaml $(TEXTURES_DIR)/tex_gui-2x.png $(TEXTURES_DIR)/tex_gui-4x.yaml $(TEXTURES_DIR)/tex_gui-4x.png guiFont Makefile

$(TEXTURES_DIR)/tex_gui.yaml $(TEXTURES_DIR)/tex_gui.png $(SRC_GEN_DIR)/tex_gui.hpp: tex_gui_desc.yaml
	@echo tex_gui...
	@$(texmake) --input=tex_gui_desc.yaml --deps=$(DEPS_DIR)/tex_gui_desc.yaml.d -o $(TEXTURES_DIR)/tex_gui.yaml $(TEXTURES_DIR)/tex_gui.png $(SRC_GEN_DIR)/tex_gui.hpp



