UNAME = $(shell uname -s)

ifneq "" "$(findstring CYGWIN,$(UNAME))"
	HOMEDIR=/cygdrive/c
	LOCALDIR=C:
else
	HOMEDIR=${HOME}
	LOCALDIR=${HOMEDIR}
endif

SRCHOMEDIR=${HOMEDIR}/src/mangl/tools/mangl-animator
RESOURCESDIR=${HOMEDIR}/Dropbox/src/mangl/animator


IMAGESDIR=${RESOURCESDIR}/textures
GRAPHICSINIDIR=${SRCHOMEDIR}/res

DEPSDIR=.deps
BUNDLEDIR=bundle


TEX = tex_animator

include $(HOMEDIR)/src/apps/tools/make/build.mk
