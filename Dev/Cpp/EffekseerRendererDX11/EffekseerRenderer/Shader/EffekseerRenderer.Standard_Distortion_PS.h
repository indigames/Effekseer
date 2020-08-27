#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//
//   fxc /Zpc /Tps_4_0 /Emain /D __EFFEKSEER_BUILD_VERSION16__=1 /Fh
//    Shader/EffekseerRenderer.Standard_Distortion_PS.h
//    Shader/standard_renderer_distortion_PS.fx
//
//
// Buffer Definitions: 
//
// cbuffer PS_ConstanBuffer
// {
//
//   float4 g_scale;                    // Offset:    0 Size:    16
//   float4 mUVInversedBack;            // Offset:   16 Size:    16
//   float4 flipbookParameter;          // Offset:   32 Size:    16
//   float4 uvDistortionParameter;      // Offset:   48 Size:    16
//   float4 blendTextureParameter;      // Offset:   64 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// g_sampler                         sampler      NA          NA    0        1
// g_backSampler                     sampler      NA          NA    1        1
// g_alphaSampler                    sampler      NA          NA    2        1
// g_uvDistortionSampler             sampler      NA          NA    3        1
// g_blendSampler                    sampler      NA          NA    4        1
// g_blendAlphaSampler               sampler      NA          NA    5        1
// g_blendUVDistortionSampler        sampler      NA          NA    6        1
// g_texture                         texture  float4          2d    0        1
// g_backTexture                     texture  float4          2d    1        1
// g_alphaTexture                    texture  float4          2d    2        1
// g_uvDistortionTexture             texture  float4          2d    3        1
// g_blendTexture                    texture  float4          2d    4        1
// g_blendAlphaTexture               texture  float4          2d    5        1
// g_blendUVDistortionTexture        texture  float4          2d    6        1
// PS_ConstanBuffer                  cbuffer      NA          NA    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_POSITION              0   xyzw        0      POS  float       
// COLOR                    0   xyzw        1     NONE  float   xy w
// TEXCOORD                 0   xy          2     NONE  float   xy  
// TEXCOORD                 7     zw        2     NONE  float     zw
// TEXCOORD                 1   xyzw        3     NONE  float   xy w
// TEXCOORD                 2   xyzw        4     NONE  float   xy w
// TEXCOORD                 3   xyzw        5     NONE  float   xy w
// TEXCOORD                 4   xyzw        6     NONE  float   xyzw
// TEXCOORD                 5   xyzw        7     NONE  float   xyzw
// TEXCOORD                 6   xyzw        8     NONE  float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue Format   Used
// -------------------- ----- ------ -------- -------- ------ ------
// SV_Target                0   xyzw        0   TARGET  float   xyzw
//
ps_4_0
dcl_constantbuffer cb0[5], immediateIndexed
dcl_sampler s0, mode_default
dcl_sampler s1, mode_default
dcl_sampler s2, mode_default
dcl_sampler s3, mode_default
dcl_sampler s4, mode_default
dcl_sampler s5, mode_default
dcl_sampler s6, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_resource_texture2d (float,float,float,float) t2
dcl_resource_texture2d (float,float,float,float) t3
dcl_resource_texture2d (float,float,float,float) t4
dcl_resource_texture2d (float,float,float,float) t5
dcl_resource_texture2d (float,float,float,float) t6
dcl_input_ps linear v1.xyw
dcl_input_ps linear v2.xy
dcl_input_ps linear v2.zw
dcl_input_ps linear v3.xyw
dcl_input_ps linear v4.xyw
dcl_input_ps linear v5.xyw
dcl_input_ps linear v6.xyzw
dcl_input_ps linear v7.xyzw
dcl_input_ps linear v8.xyzw
dcl_output o0.xyzw
dcl_temps 4
sample r0.xyzw, v6.zwzz, t3.xyzw, s3
mad r0.xy, r0.xyxx, l(2.000000, 2.000000, 0.000000, 0.000000), l(-1.000000, -1.000000, 0.000000, 0.000000)
mad r0.z, -r0.y, cb0[3].w, cb0[3].z
mad r0.yw, r0.xxxz, cb0[3].xxxx, v2.xxxy
sample r1.xyzw, r0.ywyy, t0.xyzw, s0
mul r1.z, r1.w, v1.w
lt r0.y, l(0.000000), cb0[2].x
if_nz r0.y
  mad r0.yw, r0.xxxz, cb0[3].xxxx, v8.zzzw
  sample r2.xyzw, r0.ywyy, t0.xyzw, s0
  eq r0.y, cb0[2].y, l(1.000000)
  mad r2.xyz, r2.xywx, v1.xywx, -r1.xyzx
  mad r2.xyz, v2.zzzz, r2.xyzx, r1.xyzx
  movc r1.xyz, r0.yyyy, r2.xyzx, r1.xyzx
endif 
mad r0.xy, r0.xzxx, cb0[3].xxxx, v6.xyxx
sample r0.xyzw, r0.xyxx, t2.xyzw, s2
mul r0.x, r0.w, r0.x
mul r0.x, r0.x, r1.z
max r0.y, v2.w, l(0.000000)
ge r0.y, r0.y, r0.x
discard r0.y
sample r2.xyzw, v7.zwzz, t6.xyzw, s6
mad r2.xy, r2.xyxx, l(2.000000, 2.000000, 0.000000, 0.000000), l(-1.000000, -1.000000, 0.000000, 0.000000)
mad r2.z, -r2.y, cb0[3].w, cb0[3].z
mad r0.yz, r2.xxzx, cb0[3].yyyy, v8.xxyx
sample r3.xyzw, r0.yzyy, t4.xyzw, s4
mad r0.yz, r2.xxzx, cb0[3].yyyy, v7.xxyx
sample r2.xyzw, r0.yzyy, t5.xyzw, s5
mul r0.y, r2.w, r2.x
mul r0.z, r0.y, r3.w
mul r1.zw, r3.xxxy, r0.zzzz
mad r0.y, -r3.w, r0.y, l(1.000000)
mad r0.yw, r0.yyyy, r1.xxxy, r1.zzzw
mad r2.xy, r0.zzzz, r3.xyxx, r1.xyxx
mad r2.zw, -r0.zzzz, r3.xxxy, r1.xxxy
eq r3.xyzw, cb0[4].xxxx, l(0.000000, 1.000000, 2.000000, 3.000000)
mul r1.zw, r1.zzzw, r1.xxxy
movc r1.xy, r3.wwww, r1.zwzz, r1.xyxx
movc r1.xy, r3.zzzz, r2.zwzz, r1.xyxx
movc r1.xy, r3.yyyy, r2.xyxx, r1.xyxx
movc r0.yz, r3.xxxx, r0.yywy, r1.xxyx
div r1.xy, v3.xyxx, v3.wwww
div r1.zw, v4.xxxy, v4.wwww
div r2.xy, v5.xyxx, v5.wwww
mad r0.yz, r0.yyzy, l(0.000000, 2.000000, 2.000000, 0.000000), l(0.000000, -1.000000, -1.000000, 0.000000)
mul r0.yz, r0.yyzy, v1.xxyx
mul r0.yz, r0.yyzy, cb0[0].xxxx
add r2.xy, -r1.xyxx, r2.xyxx
mad r0.yw, r2.xxxy, r0.yyyy, r1.xxxy
add r1.xy, -r1.xyxx, r1.zwzz
mad r0.yz, r1.xxyx, r0.zzzz, r0.yywy
add r0.yz, r0.yyzy, l(0.000000, 1.000000, 1.000000, 0.000000)
mul r1.x, r0.y, l(0.500000)
mad r0.y, -r0.z, l(0.500000), l(1.000000)
mad r1.z, cb0[1].y, r0.y, cb0[1].x
sample r1.xyzw, r1.xzxx, t1.xyzw, s1
mov o0.xyz, r1.xyzx
mov o0.w, r0.x
ret 
// Approximately 60 instruction slots used
#endif

const BYTE g_main[] =
{
     68,  88,  66,  67, 162, 202, 
    229,  67,  95, 237,  49, 129, 
    168, 185, 129,  62, 230, 209, 
    216,  96,   1,   0,   0,   0, 
    140,  15,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    104,   4,   0,   0, 132,   5, 
      0,   0, 184,   5,   0,   0, 
     16,  15,   0,   0,  82,  68, 
     69,  70,  44,   4,   0,   0, 
      1,   0,   0,   0,   4,   3, 
      0,   0,  15,   0,   0,   0, 
     28,   0,   0,   0,   0,   4, 
    255, 255,  16,   1,   0,   0, 
    250,   3,   0,   0, 252,   1, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      6,   2,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  20,   2,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  35,   2, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     57,   2,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  72,   2,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  92,   2, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    119,   2,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 129,   2,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 143,   2, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      2,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    158,   2,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   3,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 180,   2,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   4,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 195,   2, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      5,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    215,   2,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   6,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 242,   2,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 103,  95, 
    115,  97, 109, 112, 108, 101, 
    114,   0, 103,  95,  98,  97, 
     99, 107,  83,  97, 109, 112, 
    108, 101, 114,   0, 103,  95, 
     97, 108, 112, 104,  97,  83, 
     97, 109, 112, 108, 101, 114, 
      0, 103,  95, 117, 118,  68, 
    105, 115, 116, 111, 114, 116, 
    105, 111, 110,  83,  97, 109, 
    112, 108, 101, 114,   0, 103, 
     95,  98, 108, 101, 110, 100, 
     83,  97, 109, 112, 108, 101, 
    114,   0, 103,  95,  98, 108, 
    101, 110, 100,  65, 108, 112, 
    104,  97,  83,  97, 109, 112, 
    108, 101, 114,   0, 103,  95, 
     98, 108, 101, 110, 100,  85, 
     86,  68, 105, 115, 116, 111, 
    114, 116, 105, 111, 110,  83, 
     97, 109, 112, 108, 101, 114, 
      0, 103,  95, 116, 101, 120, 
    116, 117, 114, 101,   0, 103, 
     95,  98,  97,  99, 107,  84, 
    101, 120, 116, 117, 114, 101, 
      0, 103,  95,  97, 108, 112, 
    104,  97,  84, 101, 120, 116, 
    117, 114, 101,   0, 103,  95, 
    117, 118,  68, 105, 115, 116, 
    111, 114, 116, 105, 111, 110, 
     84, 101, 120, 116, 117, 114, 
    101,   0, 103,  95,  98, 108, 
    101, 110, 100,  84, 101, 120, 
    116, 117, 114, 101,   0, 103, 
     95,  98, 108, 101, 110, 100, 
     65, 108, 112, 104,  97,  84, 
    101, 120, 116, 117, 114, 101, 
      0, 103,  95,  98, 108, 101, 
    110, 100,  85,  86,  68, 105, 
    115, 116, 111, 114, 116, 105, 
    111, 110,  84, 101, 120, 116, 
    117, 114, 101,   0,  80,  83, 
     95,  67, 111, 110, 115, 116, 
     97, 110,  66, 117, 102, 102, 
    101, 114,   0, 171, 242,   2, 
      0,   0,   5,   0,   0,   0, 
     28,   3,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 148,   3, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 156,   3,   0,   0, 
      0,   0,   0,   0, 172,   3, 
      0,   0,  16,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 156,   3,   0,   0, 
      0,   0,   0,   0, 188,   3, 
      0,   0,  32,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 156,   3,   0,   0, 
      0,   0,   0,   0, 206,   3, 
      0,   0,  48,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 156,   3,   0,   0, 
      0,   0,   0,   0, 228,   3, 
      0,   0,  64,   0,   0,   0, 
     16,   0,   0,   0,   2,   0, 
      0,   0, 156,   3,   0,   0, 
      0,   0,   0,   0, 103,  95, 
    115,  99,  97, 108, 101,   0, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 109,  85, 
     86,  73, 110, 118, 101, 114, 
    115, 101, 100,  66,  97,  99, 
    107,   0, 102, 108, 105, 112, 
     98, 111, 111, 107,  80,  97, 
    114,  97, 109, 101, 116, 101, 
    114,   0, 117, 118,  68, 105, 
    115, 116, 111, 114, 116, 105, 
    111, 110,  80,  97, 114,  97, 
    109, 101, 116, 101, 114,   0, 
     98, 108, 101, 110, 100,  84, 
    101, 120, 116, 117, 114, 101, 
     80,  97, 114,  97, 109, 101, 
    116, 101, 114,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  57,  46,  50, 
     57,  46,  57,  53,  50,  46, 
     51,  49,  49,  49,   0, 171, 
     73,  83,  71,  78,  20,   1, 
      0,   0,  10,   0,   0,   0, 
      8,   0,   0,   0, 248,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,   4,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,  11,   0,   0,  10,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,   3,   0,   0,  10,   1, 
      0,   0,   7,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
     12,  12,   0,   0,  10,   1, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,  11,   0,   0,  10,   1, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
     15,  11,   0,   0,  10,   1, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   5,   0,   0,   0, 
     15,  11,   0,   0,  10,   1, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   6,   0,   0,   0, 
     15,  15,   0,   0,  10,   1, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   7,   0,   0,   0, 
     15,  15,   0,   0,  10,   1, 
      0,   0,   6,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     15,  15,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  67,  79, 
     76,  79,  82,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  97, 114, 
    103, 101, 116,   0, 171, 171, 
     83,  72,  68,  82,  80,   9, 
      0,   0,  64,   0,   0,   0, 
     84,   2,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   5,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   1,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   2,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   3,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   4,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   5,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   6,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   1,   0,   0,   0, 
     85,  85,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      2,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   3,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   4,   0,   0,   0, 
     85,  85,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      5,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   6,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3, 178,  16, 
     16,   0,   1,   0,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   2,   0,   0,   0, 
     98,  16,   0,   3, 194,  16, 
     16,   0,   2,   0,   0,   0, 
     98,  16,   0,   3, 178,  16, 
     16,   0,   3,   0,   0,   0, 
     98,  16,   0,   3, 178,  16, 
     16,   0,   4,   0,   0,   0, 
     98,  16,   0,   3, 178,  16, 
     16,   0,   5,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   6,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   7,   0,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   8,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   4,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0, 230,  26,  16,   0, 
      6,   0,   0,   0,  70, 126, 
     16,   0,   3,   0,   0,   0, 
      0,  96,  16,   0,   3,   0, 
      0,   0,  50,   0,   0,  15, 
     50,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  64, 
      0,   0,   0,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128, 191,   0,   0, 128, 191, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  50,   0,   0,  12, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,  58, 128,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  42, 128,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,  50,   0,   0,  10, 
    162,   0,  16,   0,   0,   0, 
      0,   0,   6,   8,  16,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   6,  20, 
     16,   0,   2,   0,   0,   0, 
     69,   0,   0,   9, 242,   0, 
     16,   0,   1,   0,   0,   0, 
    214,   5,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  58,  16,  16,   0, 
      1,   0,   0,   0,  49,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     10, 128,  32,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     31,   0,   4,   3,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 162,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   8,  16,   0,   0,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0, 166,  30,  16,   0, 
      8,   0,   0,   0,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      2,   0,   0,   0, 214,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  24,   0, 
      0,   8,  34,   0,  16,   0, 
      0,   0,   0,   0,  26, 128, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   3,  16,   0,   2,   0, 
      0,   0,  70,  19,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9, 114,   0,  16,   0, 
      2,   0,   0,   0, 166,  26, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  55,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1,  50,   0,   0,  10, 
     50,   0,  16,   0,   0,   0, 
      0,   0, 134,   0,  16,   0, 
      0,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  70,  16, 
     16,   0,   6,   0,   0,   0, 
     69,   0,   0,   9, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      2,   0,   0,   0,   0,  96, 
     16,   0,   2,   0,   0,   0, 
     56,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  52,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  58,  16,  16,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     29,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  13,   0, 
      4,   3,  26,   0,  16,   0, 
      0,   0,   0,   0,  69,   0, 
      0,   9, 242,   0,  16,   0, 
      2,   0,   0,   0, 230,  26, 
     16,   0,   7,   0,   0,   0, 
     70, 126,  16,   0,   6,   0, 
      0,   0,   0,  96,  16,   0, 
      6,   0,   0,   0,  50,   0, 
      0,  15,  50,   0,  16,   0, 
      2,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,  64,   0,   0,   0,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128, 191,   0,   0, 
    128, 191,   0,   0,   0,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  12,  66,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,  58, 128, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  42, 128, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  50,   0, 
      0,  10,  98,   0,  16,   0, 
      0,   0,   0,   0,   6,   2, 
     16,   0,   2,   0,   0,   0, 
     86, 133,  32,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      6,  17,  16,   0,   8,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   3,   0, 
      0,   0, 150,   5,  16,   0, 
      0,   0,   0,   0,  70, 126, 
     16,   0,   4,   0,   0,   0, 
      0,  96,  16,   0,   4,   0, 
      0,   0,  50,   0,   0,  10, 
     98,   0,  16,   0,   0,   0, 
      0,   0,   6,   2,  16,   0, 
      2,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   6,  17, 
     16,   0,   7,   0,   0,   0, 
     69,   0,   0,   9, 242,   0, 
     16,   0,   2,   0,   0,   0, 
    150,   5,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      5,   0,   0,   0,   0,  96, 
     16,   0,   5,   0,   0,   0, 
     56,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   7,  66,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   3,   0, 
      0,   0,  56,   0,   0,   7, 
    194,   0,  16,   0,   1,   0, 
      0,   0,   6,   4,  16,   0, 
      3,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   3,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  50,   0, 
      0,   9, 162,   0,  16,   0, 
      0,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
      6,   4,  16,   0,   1,   0, 
      0,   0, 166,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9,  50,   0,  16,   0, 
      2,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70,   0,  16,   0,   3,   0, 
      0,   0,  70,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 194,   0,  16,   0, 
      2,   0,   0,   0, 166,  10, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   6,   4, 
     16,   0,   3,   0,   0,   0, 
      6,   4,  16,   0,   1,   0, 
      0,   0,  24,   0,   0,  11, 
    242,   0,  16,   0,   3,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0,   0,  64, 
      0,   0,  64,  64,  56,   0, 
      0,   7, 194,   0,  16,   0, 
      1,   0,   0,   0, 166,  14, 
     16,   0,   1,   0,   0,   0, 
      6,   4,  16,   0,   1,   0, 
      0,   0,  55,   0,   0,   9, 
     50,   0,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      3,   0,   0,   0, 230,  10, 
     16,   0,   1,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  55,   0,   0,   9, 
     50,   0,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      3,   0,   0,   0, 230,  10, 
     16,   0,   2,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  55,   0,   0,   9, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      3,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   0,  16,   0,   1,   0, 
      0,   0,  55,   0,   0,   9, 
     98,   0,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      3,   0,   0,   0,  86,   7, 
     16,   0,   0,   0,   0,   0, 
      6,   1,  16,   0,   1,   0, 
      0,   0,  14,   0,   0,   7, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,  16,  16,   0, 
      3,   0,   0,   0, 246,  31, 
     16,   0,   3,   0,   0,   0, 
     14,   0,   0,   7, 194,   0, 
     16,   0,   1,   0,   0,   0, 
      6,  20,  16,   0,   4,   0, 
      0,   0, 246,  31,  16,   0, 
      4,   0,   0,   0,  14,   0, 
      0,   7,  50,   0,  16,   0, 
      2,   0,   0,   0,  70,  16, 
     16,   0,   5,   0,   0,   0, 
    246,  31,  16,   0,   5,   0, 
      0,   0,  50,   0,   0,  15, 
     98,   0,  16,   0,   0,   0, 
      0,   0,  86,   6,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,  64,   0,   0, 
      0,  64,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0, 128, 191, 
      0,   0, 128, 191,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     98,   0,  16,   0,   0,   0, 
      0,   0,  86,   6,  16,   0, 
      0,   0,   0,   0,   6,  17, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   8,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,   6, 128,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   8, 
     50,   0,  16,   0,   2,   0, 
      0,   0,  70,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  70,   0,  16,   0, 
      2,   0,   0,   0,  50,   0, 
      0,   9, 162,   0,  16,   0, 
      0,   0,   0,   0,   6,   4, 
     16,   0,   2,   0,   0,   0, 
     86,   5,  16,   0,   0,   0, 
      0,   0,   6,   4,  16,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   8,  50,   0,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0, 230,  10, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9,  98,   0, 
     16,   0,   0,   0,   0,   0, 
      6,   1,  16,   0,   1,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  86,   7, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,  10,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,  63,  50,   0,   0,  10, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     50,   0,   0,  11,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     26, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   1,   0, 
      0,   0, 134,   0,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   1,   0,   0,   0, 
      0,  96,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    114,  32,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  54,   0, 
      0,   5, 130,  32,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 116,   0,   0,   0, 
     60,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     10,   0,   0,   0,  21,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0
};
