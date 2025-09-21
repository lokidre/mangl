# *** Automatically Generated *** DO NOT EDIT ***
# * Generated Date: 2024-11-18

include ../../../.build-config.make

TEXTURES_DIR := $(CONFIG_MANGL_SAMPLES_ASSETS_DIR)/textures
RENDER_DIR := $(CONFIG_MANGL_SAMPLES_ASSETS_DIR)/render
RENDER_FONT_DIR := $(CONFIG_MANGL_SAMPLES_ASSETS_DIR)/render/fonts
FONTS_DIR := $(CONFIG_MANGL_ASSETS_DIR)/fonts
SRC_GEN_DIR := $(CONFIG_MANGL_SAMPLES_DIR)/common/generated

DEPS_DIR := .deps

texmake := mangl-make-tex
fontmake := mangl-font-render

FONTMAKE_PARAMS :=
TEXMAKE_PARAMS := 

fonts := guiFont

textures := tex_common tex_tutorial

all: $(fonts) $(textures)

.PHONY: all $(fonts) $(textures)

-include $(DEPS_DIR)/tex_common_desc.yaml.d
-include $(DEPS_DIR)/tex_tutorial_desc.yaml.d

$(DEPS_DIR): ; @mkdir -p $@

Makefile: samples-graphics.yaml
	zx genbuild.mjs

#
# Generating Fonts
#

guiFont: $(RENDER_FONT_DIR)/guiFont.yaml $(RENDER_FONT_DIR)/guiFont.png

$(RENDER_FONT_DIR)/guiFont.yaml $(RENDER_FONT_DIR)/guiFont.png: Makefile
	@echo guiFont...
	@$(fontmake) $(FONTMAKE_PARAMS) --input=$(FONTS_DIR)/Refrigerator_Deluxe_Bold.otf --height=32 -o $(RENDER_FONT_DIR)/guiFont.yaml $(RENDER_FONT_DIR)/guiFont.png


#
# Generating Textures
#

tex_common: $(TEXTURES_DIR)/tex_common.yaml $(TEXTURES_DIR)/tex_common.png $(SRC_GEN_DIR)/tex_common.hpp guiFont Makefile

$(TEXTURES_DIR)/tex_common.yaml $(TEXTURES_DIR)/tex_common.png $(SRC_GEN_DIR)/tex_common.hpp: tex_common_desc.yaml
	@echo tex_common...
	@$(texmake) $(TEXMAKE_PARAMS) --input=tex_common_desc.yaml --deps=$(DEPS_DIR)/tex_common_desc.yaml.d --define=ROOT=$(CONFIG_MANGL_SAMPLES_ASSETS_DIR) -o $(TEXTURES_DIR)/tex_common.yaml $(TEXTURES_DIR)/tex_common.png $(SRC_GEN_DIR)/tex_common.hpp


tex_tutorial: $(TEXTURES_DIR)/tex_tutorial.yaml $(TEXTURES_DIR)/tex_tutorial.png $(SRC_GEN_DIR)/tex_tutorial.hpp guiFont Makefile

$(TEXTURES_DIR)/tex_tutorial.yaml $(TEXTURES_DIR)/tex_tutorial.png $(SRC_GEN_DIR)/tex_tutorial.hpp: tex_tutorial_desc.yaml
	@echo tex_tutorial...
	@$(texmake) $(TEXMAKE_PARAMS) --input=tex_tutorial_desc.yaml --deps=$(DEPS_DIR)/tex_tutorial_desc.yaml.d --define=ROOT=$(CONFIG_MANGL_SAMPLES_ASSETS_DIR) -o $(TEXTURES_DIR)/tex_tutorial.yaml $(TEXTURES_DIR)/tex_tutorial.png $(SRC_GEN_DIR)/tex_tutorial.hpp



