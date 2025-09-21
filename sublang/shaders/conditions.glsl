#line 0

#ifdef COND_SHADING_ON
#define COND_SHADING_BEGIN  {
#elif defined COND_SHADING_OFF
#define COND_SHADING_BEGIN  if (false) {
#else
#define COND_SHADING_BEGIN  if (shading_enabled) {
#endif
#define COND_SHADING_END    }

//#define COND_SHADING_FLAT
//#define COND_SHADING_GOURAUD
#define COND_SHADING_PHONG



#ifdef COND_TEX_ON
#define COND_TEX_BEGIN      {
#elif defined COND_TEX_OFF
#define COND_TEX_BEGIN      if (false) {
#else
#define COND_TEX_BEGIN      if (tex_enabled) {
#endif
#define COND_TEX_END        }


#ifdef COND_FOG_ON
#define COND_FOG_BEGIN      {
#elif defined COND_FOG_OFF
#define COND_FOG_BEGIN      if (false) {
#else  // uniform
#define COND_FOG_BEGIN      if (fog_enabled) {
#endif
#define COND_FOG_END        }

#define FOG_UNI_PACKED      1


#ifdef COND_LINE_ON
#define COND_LINE_BEGIN     {
#elif defined COND_LINE_OFF
#define COND_LINE_BEGIN     if (false) {
#else
#define COND_LINE_BEGIN     if (prim_type == 2) {
#endif
#define COND_LINE_END       }

#define LINE_UNI_PACKED     1


#ifdef COND_POINT_ON
#define COND_POINT_BEGIN    {
#elif defined COND_POINT_OFF
#define COND_POINT_BEGIN    if (false) {
#else
#define COND_POINT_BEGIN    if (prim_type == 3) {
#endif
#define COND_POINT_END      }

