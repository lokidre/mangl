#! /bin/bash

SHADER_SOURCE="../../src/core/opengl/mangl_opengl_shader_sources.cpp"
SHADER_HEADER="../../src/core/opengl/mangl_opengl_shader_sources.h"

echo "// +++ DO NOT EDIT +++ Automatically Generated File +++" > $SHADER_SOURCE
echo "namespace mangl::opengl {" >> $SHADER_SOURCE

echo "// +++ DO NOT EDIT +++ Automatically Generated File +++" > $SHADER_HEADER
echo "#pragma once" >> $SHADER_HEADER
echo "namespace mangl::opengl {" >> $SHADER_HEADER



for shader in *.glsl ; do
    filename="${shader%.*}"
    varname="shader_${filename}_"

    echo "extern const char *${varname};" >> $SHADER_HEADER

    echo "const char *${varname} = R\"===(" >> $SHADER_SOURCE
    cat $shader >> $SHADER_SOURCE
    echo ")===\" ;" >> $SHADER_SOURCE
done

echo "}  // namespace mangl::opengl" >> $SHADER_SOURCE
echo "}  // namespace mangl::opengl" >> $SHADER_HEADER

echo "Shader Sources generated"
exit 0
