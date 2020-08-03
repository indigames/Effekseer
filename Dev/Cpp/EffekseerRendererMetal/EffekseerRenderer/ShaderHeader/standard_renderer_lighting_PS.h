static const char metal_standard_renderer_lighting_PS[] = R"(mtlcode
#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct PS_Input
{
    float4 Position;
    float4 VColor;
    float2 UV1;
    float2 UV2;
    float3 WorldP;
    float3 WorldN;
    float3 WorldT;
    float3 WorldB;
    float2 ScreenUV;
    float2 AlphaUV;
    float2 UVDistortionUV;
    float2 BlendUV;
    float2 BlendAlphaUV;
    float2 BlendUVDistortionUV;
    float FlipbookRate;
    float2 FlipbookNextIndexUV;
    float AlphaThreshold;
};

struct PS_ConstanBuffer
{
    float4 fLightDirection;
    float4 fLightColor;
    float4 fLightAmbient;
    float4 fFlipbookParameter;
    float4 fUVDistortionParameter;
    float4 fBlendTextureParameter;
    float4 fEmissiveScaling;
    float4 fEdgeColor;
    float4 fEdgeParameter;
};

struct main0_out
{
    float4 _entryPointOutput [[color(0)]];
};

struct main0_in
{
    float4 Input_VColor [[user(locn0)]];
    float2 Input_UV1 [[user(locn1)]];
    float2 Input_UV2 [[user(locn2)]];
    float3 Input_WorldP [[user(locn3)]];
    float3 Input_WorldN [[user(locn4)]];
    float3 Input_WorldT [[user(locn5)]];
    float3 Input_WorldB [[user(locn6)]];
    float2 Input_ScreenUV [[user(locn7)]];
    float2 Input_AlphaUV [[user(locn8)]];
    float2 Input_UVDistortionUV [[user(locn9)]];
    float2 Input_BlendUV [[user(locn10)]];
    float2 Input_BlendAlphaUV [[user(locn11)]];
    float2 Input_BlendUVDistortionUV [[user(locn12)]];
    float Input_FlipbookRate [[user(locn13)]];
    float2 Input_FlipbookNextIndexUV [[user(locn14)]];
    float Input_AlphaThreshold [[user(locn15)]];
};

static inline __attribute__((always_inline))
void ApplyFlipbook(thread float4& dst, texture2d<float> t, sampler s, float4 flipbookParameter, float4 vcolor, float2 nextUV, thread const float& flipbookRate)
{
    if (flipbookParameter.x > 0.0)
    {
        float4 NextPixelColor = t.sample(s, nextUV) * vcolor;
        if (flipbookParameter.y == 1.0)
        {
            dst = mix(dst, NextPixelColor, float4(flipbookRate));
        }
    }
}

static inline __attribute__((always_inline))
void ApplyTextureBlending(thread float4& dstColor, float4 blendColor, float blendType)
{
    if (blendType == 0.0)
    {
        float3 _71 = (blendColor.xyz * blendColor.w) + (dstColor.xyz * (1.0 - blendColor.w));
        dstColor = float4(_71.x, _71.y, _71.z, dstColor.w);
    }
    else
    {
        if (blendType == 1.0)
        {
            float3 _83 = dstColor.xyz + (blendColor.xyz * blendColor.w);
            dstColor = float4(_83.x, _83.y, _83.z, dstColor.w);
        }
        else
        {
            if (blendType == 2.0)
            {
                float3 _96 = dstColor.xyz - (blendColor.xyz * blendColor.w);
                dstColor = float4(_96.x, _96.y, _96.z, dstColor.w);
            }
            else
            {
                if (blendType == 3.0)
                {
                    float3 _109 = dstColor.xyz * (blendColor.xyz * blendColor.w);
                    dstColor = float4(_109.x, _109.y, _109.z, dstColor.w);
                }
            }
        }
    }
}

static inline __attribute__((always_inline))
float4 _main(PS_Input Input, thread texture2d<float> g_normalTexture, thread sampler g_normalSampler, constant PS_ConstanBuffer& v_159, thread texture2d<float> g_uvDistortionTexture, thread sampler g_uvDistortionSampler, thread texture2d<float> g_colorTexture, thread sampler g_colorSampler, thread texture2d<float> g_alphaTexture, thread sampler g_alphaSampler, thread texture2d<float> g_blendUVDistortionTexture, thread sampler g_blendUVDistortionSampler, thread texture2d<float> g_blendTexture, thread sampler g_blendSampler, thread texture2d<float> g_blendAlphaTexture, thread sampler g_blendAlphaSampler)
{
    float diffuse = 1.0;
    float3 loN = g_normalTexture.sample(g_normalSampler, Input.UV1).xyz;
    float3 texNormal = (loN - float3(0.5)) * 2.0;
    float3 localNormal = normalize(float3x3(float3(Input.WorldT), float3(Input.WorldB), float3(Input.WorldN)) * texNormal);
    diffuse = fast::max(dot(v_159.fLightDirection.xyz, localNormal), 0.0);
    float2 UVOffset = float2(0.0);
    UVOffset = (g_uvDistortionTexture.sample(g_uvDistortionSampler, Input.UVDistortionUV).xy * 2.0) - float2(1.0);
    UVOffset *= v_159.fUVDistortionParameter.x;
    float4 Output = g_colorTexture.sample(g_colorSampler, (Input.UV1 + UVOffset)) * Input.VColor;
    float4 param = Output;
    float param_1 = Input.FlipbookRate;
    ApplyFlipbook(param, g_colorTexture, g_colorSampler, v_159.fFlipbookParameter, Input.VColor, Input.FlipbookNextIndexUV + UVOffset, param_1);
    Output = param;
    float4 AlphaTexColor = g_alphaTexture.sample(g_alphaSampler, (Input.AlphaUV + UVOffset));
    Output.w *= (AlphaTexColor.x * AlphaTexColor.w);
    float2 BlendUVOffset = (g_blendUVDistortionTexture.sample(g_blendUVDistortionSampler, Input.BlendUVDistortionUV).xy * 2.0) - float2(1.0);
    BlendUVOffset *= v_159.fUVDistortionParameter.y;
    float4 BlendTextureColor = g_blendTexture.sample(g_blendSampler, (Input.BlendUV + BlendUVOffset));
    float4 BlendAlphaTextureColor = g_blendAlphaTexture.sample(g_blendAlphaSampler, (Input.BlendAlphaUV + BlendUVOffset));
    BlendTextureColor.w *= (BlendAlphaTextureColor.x * BlendAlphaTextureColor.w);
    float4 param_2 = Output;
    ApplyTextureBlending(param_2, BlendTextureColor, v_159.fBlendTextureParameter.x);
    Output = param_2;
    float3 _298 = Output.xyz * v_159.fEmissiveScaling.x;
    Output = float4(_298.x, _298.y, _298.z, Output.w);
    if (Output.w <= Input.AlphaThreshold)
    {
        discard_fragment();
    }
    float3 _322 = Output.xyz * (float3(diffuse, diffuse, diffuse) + float3(v_159.fLightAmbient.xyz));
    Output = float4(_322.x, _322.y, _322.z, Output.w);
    float3 _343 = mix(v_159.fEdgeColor.xyz * v_159.fEdgeParameter.y, Output.xyz, float3(ceil((Output.w - Input.AlphaThreshold) - v_159.fEdgeParameter.x)));
    Output = float4(_343.x, _343.y, _343.z, Output.w);
    if (Output.w == 0.0)
    {
        discard_fragment();
    }
    return Output;
}

fragment main0_out main0(main0_in in [[stage_in]], constant PS_ConstanBuffer& v_159 [[buffer(0)]], texture2d<float> g_normalTexture [[texture(0)]], texture2d<float> g_uvDistortionTexture [[texture(1)]], texture2d<float> g_colorTexture [[texture(2)]], texture2d<float> g_alphaTexture [[texture(3)]], texture2d<float> g_blendUVDistortionTexture [[texture(4)]], texture2d<float> g_blendTexture [[texture(5)]], texture2d<float> g_blendAlphaTexture [[texture(6)]], sampler g_normalSampler [[sampler(0)]], sampler g_uvDistortionSampler [[sampler(1)]], sampler g_colorSampler [[sampler(2)]], sampler g_alphaSampler [[sampler(3)]], sampler g_blendUVDistortionSampler [[sampler(4)]], sampler g_blendSampler [[sampler(5)]], sampler g_blendAlphaSampler [[sampler(6)]], float4 gl_FragCoord [[position]])
{
    main0_out out = {};
    PS_Input Input;
    Input.Position = gl_FragCoord;
    Input.VColor = in.Input_VColor;
    Input.UV1 = in.Input_UV1;
    Input.UV2 = in.Input_UV2;
    Input.WorldP = in.Input_WorldP;
    Input.WorldN = in.Input_WorldN;
    Input.WorldT = in.Input_WorldT;
    Input.WorldB = in.Input_WorldB;
    Input.ScreenUV = in.Input_ScreenUV;
    Input.AlphaUV = in.Input_AlphaUV;
    Input.UVDistortionUV = in.Input_UVDistortionUV;
    Input.BlendUV = in.Input_BlendUV;
    Input.BlendAlphaUV = in.Input_BlendAlphaUV;
    Input.BlendUVDistortionUV = in.Input_BlendUVDistortionUV;
    Input.FlipbookRate = in.Input_FlipbookRate;
    Input.FlipbookNextIndexUV = in.Input_FlipbookNextIndexUV;
    Input.AlphaThreshold = in.Input_AlphaThreshold;
    float4 _415 = _main(Input, g_normalTexture, g_normalSampler, v_159, g_uvDistortionTexture, g_uvDistortionSampler, g_colorTexture, g_colorSampler, g_alphaTexture, g_alphaSampler, g_blendUVDistortionTexture, g_blendUVDistortionSampler, g_blendTexture, g_blendSampler, g_blendAlphaTexture, g_blendAlphaSampler);
    out._entryPointOutput = _415;
    return out;
}

)";

