#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct PS_Input
{
    float4 Position;
    float4 VColor;
    float2 UV;
    float3 WorldP;
    float3 WorldN;
    float3 WorldT;
    float3 WorldB;
    float4 Alpha_Dist_UV;
    float4 Blend_Alpha_Dist_UV;
    float4 Blend_FBNextIndex_UV;
    float2 Others;
};

struct AdvancedParameter
{
    float2 AlphaUV;
    float2 UVDistortionUV;
    float2 BlendUV;
    float2 BlendAlphaUV;
    float2 BlendUVDistortionUV;
    float2 FlipbookNextIndexUV;
    float FlipbookRate;
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
    float2 Input_UV [[user(locn1)]];
    float3 Input_WorldP [[user(locn2)]];
    float3 Input_WorldN [[user(locn3)]];
    float3 Input_WorldT [[user(locn4)]];
    float3 Input_WorldB [[user(locn5)]];
    float4 Input_Alpha_Dist_UV [[user(locn6)]];
    float4 Input_Blend_Alpha_Dist_UV [[user(locn7)]];
    float4 Input_Blend_FBNextIndex_UV [[user(locn8)]];
    float2 Input_Others [[user(locn9)]];
};

static inline __attribute__((always_inline))
AdvancedParameter DisolveAdvancedParameter(thread const PS_Input& psinput)
{
    AdvancedParameter ret;
    ret.AlphaUV = psinput.Alpha_Dist_UV.xy;
    ret.UVDistortionUV = psinput.Alpha_Dist_UV.zw;
    ret.BlendUV = psinput.Blend_FBNextIndex_UV.xy;
    ret.BlendAlphaUV = psinput.Blend_Alpha_Dist_UV.xy;
    ret.BlendUVDistortionUV = psinput.Blend_Alpha_Dist_UV.zw;
    ret.FlipbookNextIndexUV = psinput.Blend_FBNextIndex_UV.zw;
    ret.FlipbookRate = psinput.Others.x;
    ret.AlphaThreshold = psinput.Others.y;
    return ret;
}

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
        float3 _77 = (blendColor.xyz * blendColor.w) + (dstColor.xyz * (1.0 - blendColor.w));
        dstColor = float4(_77.x, _77.y, _77.z, dstColor.w);
    }
    else
    {
        if (blendType == 1.0)
        {
            float3 _89 = dstColor.xyz + (blendColor.xyz * blendColor.w);
            dstColor = float4(_89.x, _89.y, _89.z, dstColor.w);
        }
        else
        {
            if (blendType == 2.0)
            {
                float3 _102 = dstColor.xyz - (blendColor.xyz * blendColor.w);
                dstColor = float4(_102.x, _102.y, _102.z, dstColor.w);
            }
            else
            {
                if (blendType == 3.0)
                {
                    float3 _115 = dstColor.xyz * (blendColor.xyz * blendColor.w);
                    dstColor = float4(_115.x, _115.y, _115.z, dstColor.w);
                }
            }
        }
    }
}

static inline __attribute__((always_inline))
float4 _main(PS_Input Input, thread texture2d<float> g_uvDistortionTexture, thread sampler g_uvDistortionSampler, constant PS_ConstanBuffer& v_187, thread texture2d<float> g_normalTexture, thread sampler g_normalSampler, thread texture2d<float> g_colorTexture, thread sampler g_colorSampler, thread texture2d<float> g_alphaTexture, thread sampler g_alphaSampler, thread texture2d<float> g_blendUVDistortionTexture, thread sampler g_blendUVDistortionSampler, thread texture2d<float> g_blendTexture, thread sampler g_blendSampler, thread texture2d<float> g_blendAlphaTexture, thread sampler g_blendAlphaSampler)
{
    PS_Input param = Input;
    AdvancedParameter advancedParam = DisolveAdvancedParameter(param);
    float2 UVOffset = float2(0.0);
    UVOffset = (g_uvDistortionTexture.sample(g_uvDistortionSampler, advancedParam.UVDistortionUV).xy * 2.0) - float2(1.0);
    UVOffset *= v_187.fUVDistortionParameter.x;
    float diffuse = 1.0;
    float3 loN = g_normalTexture.sample(g_normalSampler, (Input.UV + UVOffset)).xyz;
    float3 texNormal = (loN - float3(0.5)) * 2.0;
    float3 localNormal = normalize(float3x3(float3(Input.WorldT), float3(Input.WorldB), float3(Input.WorldN)) * texNormal);
    diffuse = fast::max(dot(v_187.fLightDirection.xyz, localNormal), 0.0);
    float4 Output = g_colorTexture.sample(g_colorSampler, (Input.UV + UVOffset)) * Input.VColor;
    float4 param_1 = Output;
    float param_2 = advancedParam.FlipbookRate;
    ApplyFlipbook(param_1, g_colorTexture, g_colorSampler, v_187.fFlipbookParameter, Input.VColor, advancedParam.FlipbookNextIndexUV + UVOffset, param_2);
    Output = param_1;
    float4 AlphaTexColor = g_alphaTexture.sample(g_alphaSampler, (advancedParam.AlphaUV + UVOffset));
    Output.w *= (AlphaTexColor.x * AlphaTexColor.w);
    float2 BlendUVOffset = (g_blendUVDistortionTexture.sample(g_blendUVDistortionSampler, advancedParam.BlendUVDistortionUV).xy * 2.0) - float2(1.0);
    BlendUVOffset *= v_187.fUVDistortionParameter.y;
    float4 BlendTextureColor = g_blendTexture.sample(g_blendSampler, (advancedParam.BlendUV + BlendUVOffset));
    float4 BlendAlphaTextureColor = g_blendAlphaTexture.sample(g_blendAlphaSampler, (advancedParam.BlendAlphaUV + BlendUVOffset));
    BlendTextureColor.w *= (BlendAlphaTextureColor.x * BlendAlphaTextureColor.w);
    float4 param_3 = Output;
    ApplyTextureBlending(param_3, BlendTextureColor, v_187.fBlendTextureParameter.x);
    Output = param_3;
    float3 _347 = Output.xyz * v_187.fEmissiveScaling.x;
    Output = float4(_347.x, _347.y, _347.z, Output.w);
    if (Output.w <= fast::max(0.0, advancedParam.AlphaThreshold))
    {
        discard_fragment();
    }
    float3 _372 = Output.xyz * (float3(diffuse, diffuse, diffuse) + float3(v_187.fLightAmbient.xyz));
    Output = float4(_372.x, _372.y, _372.z, Output.w);
    float3 _393 = mix(v_187.fEdgeColor.xyz * v_187.fEdgeParameter.y, Output.xyz, float3(ceil((Output.w - advancedParam.AlphaThreshold) - v_187.fEdgeParameter.x)));
    Output = float4(_393.x, _393.y, _393.z, Output.w);
    return Output;
}

fragment main0_out main0(main0_in in [[stage_in]], constant PS_ConstanBuffer& v_187 [[buffer(0)]], texture2d<float> g_uvDistortionTexture [[texture(0)]], texture2d<float> g_normalTexture [[texture(1)]], texture2d<float> g_colorTexture [[texture(2)]], texture2d<float> g_alphaTexture [[texture(3)]], texture2d<float> g_blendUVDistortionTexture [[texture(4)]], texture2d<float> g_blendTexture [[texture(5)]], texture2d<float> g_blendAlphaTexture [[texture(6)]], sampler g_uvDistortionSampler [[sampler(0)]], sampler g_normalSampler [[sampler(1)]], sampler g_colorSampler [[sampler(2)]], sampler g_alphaSampler [[sampler(3)]], sampler g_blendUVDistortionSampler [[sampler(4)]], sampler g_blendSampler [[sampler(5)]], sampler g_blendAlphaSampler [[sampler(6)]], float4 gl_FragCoord [[position]])
{
    main0_out out = {};
    PS_Input Input;
    Input.Position = gl_FragCoord;
    Input.VColor = in.Input_VColor;
    Input.UV = in.Input_UV;
    Input.WorldP = in.Input_WorldP;
    Input.WorldN = in.Input_WorldN;
    Input.WorldT = in.Input_WorldT;
    Input.WorldB = in.Input_WorldB;
    Input.Alpha_Dist_UV = in.Input_Alpha_Dist_UV;
    Input.Blend_Alpha_Dist_UV = in.Input_Blend_Alpha_Dist_UV;
    Input.Blend_FBNextIndex_UV = in.Input_Blend_FBNextIndex_UV;
    Input.Others = in.Input_Others;
    float4 _439 = _main(Input, g_uvDistortionTexture, g_uvDistortionSampler, v_187, g_normalTexture, g_normalSampler, g_colorTexture, g_colorSampler, g_alphaTexture, g_alphaSampler, g_blendUVDistortionTexture, g_blendUVDistortionSampler, g_blendTexture, g_blendSampler, g_blendAlphaTexture, g_blendAlphaSampler);
    out._entryPointOutput = _439;
    return out;
}

