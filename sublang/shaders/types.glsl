
#ifdef GL_ES

#ifdef IS_VERTEX
precision highp     float;
precision mediump   int;
#elif defined IS_FRAGMENT
precision mediump   float;
precision mediump   int;
#endif

#define ColorPrec   mediump
#define Byte        lowp int

#else  // Regular GLSL

// Desktop GLSL don't have precisions
#define mediump
#define lowp
#define highp

#define ColorPrec
#define Byte        int

#endif  // Regular GLSL vs ES


#define Color       ColorPrec vec4
#define ColorFloat  ColorPrec float



#if __VERSION__ < 130
#define InAtt       attribute
#define InVar       varying
#define OutVar      varying
#else  // Version >= 130
#define InAtt       in
#define InVar       in
#define OutVar      out
#endif  // Version >= 130

#define OutFloat    OutVar float
#define OutVec2     OutVar vec2
#define OutVec3     OutVar vec3
#define OutVec4     OutVar vec4
#define OutColor    OutVar Color
#define OutColorV   OutVar ColorPrec float

#define InVec2      InVar vec2
#define InVec3      InVar vec3
#define InVec4      InVar vec4
#define InColorV    InVar ColorPrec float
#define InColor     InVar Color


#define UniFloat    uniform mediump float
#define UniInt      uniform int
#define UniBool     uniform bool
#define UniVec2     uniform vec2
#define UniVec3     uniform vec3
#define UniVec4     uniform vec4
#define UniVec2I    uniform ivec2
#define UniVec3I    uniform ivec3
#define UniVec4I    uniform ivec4
#define UniMat4     uniform mat4
#define UniSampler2 uniform sampler2D

#define UniColor    uniform Color
#define UniByte     uniform Byte


#if __VERSION__ < 150
#define texture(_p1, _p2)       texture2D(_p1, _p2)
#define FRAG_OUT_DECL
#define out_frag_color          gl_FragColor
#else  // GLSL Version >= 150
#define FRAG_OUT_DECL           out Color out_frag_color;
#endif  // GLSL Version ~ 150

