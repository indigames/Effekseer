static const char metal_model_renderer_texture_PS[] = R"(mtlcode
#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct PS_Input
{
    float4 Pos;
    float2 UV;
    float3 Normal;
    float3 Binormal;
    float3 Tangent;
    float2 AlphaUV;
    float2 UVDistortionUV;
    float2 BlendUV;
    float2 BlendAlphaUV;
    float2 BlendUVDistortionUV;
    float FlipbookRate;
    float2 FlipbookNextIndexUV;
    float AlphaThreshold;
    float4 Color;
};

struct FalloffParameter
{
    float4 Param;
    float4 BeginColor;
    float4 EndColor;
};

struct PS_ConstanBuffer
{
    float4 fLightDirection;
    float4 fLightColor;
    float4 fLightAmbient;
    float4 fFlipbookParameter;
    float4 fUVDistortionParameter;
    float4 fBlendTextureParameter;
    float4 fCameraFrontDirection;
    FalloffParameter fFalloffParam;
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
    float2 Input_UV [[user(locn0)]];
    float3 Input_Normal [[user(locn1)]];
    float3 Input_Binormal [[user(locn2)]];
    float3 Input_Tangent [[user(locn3)]];
    float2 Input_AlphaUV [[user(locn4)]];
    float2 Input_UVDistortionUV [[user(locn5)]];
    float2 Input_BlendUV [[user(locn6)]];
    float2 Input_BlendAlphaUV [[user(locn7)]];
    float2 Input_BlendUVDistortionUV [[user(locn8)]];
    float Input_FlipbookRate [[user(locn9)]];
    float2 Input_FlipbookNextIndexUV [[user(locn10)]];
    float Input_AlphaThreshold [[user(locn11)]];
    float4 Input_Color [[user(locn12)]];
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
float4 _main(PS_Input Input, thread texture2d<float> g_uvDistortionTexture, thread sampler g_uvDistortionSampler, constant PS_ConstanBuffer& v_133, thread texture2d<float> g_colorTexture, thread sampler g_colorSampler, thread texture2d<float> g_alphaTexture, thread sampler g_alphaSampler, thread texture2d<float> g_blendUVDistortionTexture, thread sampler g_blendUVDistortionSampler, thread texture2d<float> g_blendTexture, thread sampler g_blendSampler, thread texture2d<float> g_blendAlphaTexture, thread sampler g_blendAlphaSampler)
{
    float2 UVOffset = float2(0.0);
    UVOffset = (g_uvDistortionTexture.sample(g_uvDistortionSampler, Input.UVDistortionUV).xy * 2.0) - float2(1.0);
    UVOffset *= v_133.fUVDistortionParameter.x;
    float4 Output = g_colorTexture.sample(g_colorSampler, (Input.UV + UVOffset)) * Input.Color;
    float4 param = Output;
    float param_1 = Input.FlipbookRate;
    ApplyFlipbook(param, g_colorTexture, g_colorSampler, v_133.fFlipbookParameter, Input.Color, Input.FlipbookNextIndexUV + UVOffset, param_1);
    Output = param;
    float4 AlphaTexColor = g_alphaTexture.sample(g_alphaSampler, (Input.AlphaUV + UVOffset));
    Output.w *= (AlphaTexColor.x * AlphaTexColor.w);
    float2 BlendUVOffset = (g_blendUVDistortionTexture.sample(g_blendUVDistortionSampler, Input.BlendUVDistortionUV).xy * 2.0) - float2(1.0);
    BlendUVOffset *= v_133.fUVDistortionParameter.y;
    float4 BlendTextureColor = g_blendTexture.sample(g_blendSampler, (Input.BlendUV + BlendUVOffset));
    float4 BlendAlphaTextureColor = g_blendAlphaTexture.sample(g_blendAlphaSampler, (Input.BlendAlphaUV + BlendUVOffset));
    BlendTextureColor.w *= (BlendAlphaTextureColor.x * BlendAlphaTextureColor.w);
    float4 param_2 = Output;
    ApplyTextureBlending(param_2, BlendTextureColor, v_133.fBlendTextureParameter.x);
    Output = param_2;
    if (v_133.fFalloffParam.Param.x == 1.0)
    {
        float3 cameraVec = normalize(-v_133.fCameraFrontDirection.xyz);
        float CdotN = fast::clamp(dot(cameraVec, normalize(Input.Normal)), 0.0, 1.0);
        float4 FalloffBlendColor = mix(v_133.fFalloffParam.EndColor, v_133.fFalloffParam.BeginColor, float4(pow(CdotN, v_133.fFalloffParam.Param.z)));
        if (v_133.fFalloffParam.Param.y == 0.0)
        {
            float3 _288 = Output.xyz + FalloffBlendColor.xyz;
            Output = float4(_288.x, _288.y, _288.z, Output.w);
        }
        else
        {
            if (v_133.fFalloffParam.Param.y == 1.0)
            {
                float3 _301 = Output.xyz - FalloffBlendColor.xyz;
                Output = float4(_301.x, _301.y, _301.z, Output.w);
            }
            else
            {
                if (v_133.fFalloffParam.Param.y == 2.0)
                {
                    float3 _314 = Output.xyz * FalloffBlendColor.xyz;
                    Output = float4(_314.x, _314.y, _314.z, Output.w);
                }
            }
        }
        Output.w *= FalloffBlendColor.w;
    }
    float3 _327 = Output.xyz * v_133.fEmissiveScaling.x;
    Output = float4(_327.x, _327.y, _327.z, Output.w);
    if (Output.w <= Input.AlphaThreshold)
    {
        discard_fragment();
    }
    float3 _355 = mix(v_133.fEdgeColor.xyz * v_133.fEdgeParameter.y, Output.xyz, float3(ceil((Output.w - Input.AlphaThreshold) - v_133.fEdgeParameter.x)));
    Output = float4(_355.x, _355.y, _355.z, Output.w);
    if (Output.w == 0.0)
    {
        discard_fragment();
    }
    return Output;
}

fragment main0_out main0(main0_in in [[stage_in]], constant PS_ConstanBuffer& v_133 [[buffer(0)]], texture2d<float> g_uvDistortionTexture [[texture(0)]], texture2d<float> g_colorTexture [[texture(1)]], texture2d<float> g_alphaTexture [[texture(2)]], texture2d<float> g_blendUVDistortionTexture [[texture(3)]], texture2d<float> g_blendTexture [[texture(4)]], texture2d<float> g_blendAlphaTexture [[texture(5)]], sampler g_uvDistortionSampler [[sampler(0)]], sampler g_colorSampler [[sampler(1)]], sampler g_alphaSampler [[sampler(2)]], sampler g_blendUVDistortionSampler [[sampler(3)]], sampler g_blendSampler [[sampler(4)]], sampler g_blendAlphaSampler [[sampler(5)]], float4 gl_FragCoord [[position]])
{
    main0_out out = {};
    PS_Input Input;
    Input.Pos = gl_FragCoord;
    Input.UV = in.Input_UV;
    Input.Normal = in.Input_Normal;
    Input.Binormal = in.Input_Binormal;
    Input.Tangent = in.Input_Tangent;
    Input.AlphaUV = in.Input_AlphaUV;
    Input.UVDistortionUV = in.Input_UVDistortionUV;
    Input.BlendUV = in.Input_BlendUV;
    Input.BlendAlphaUV = in.Input_BlendAlphaUV;
    Input.BlendUVDistortionUV = in.Input_BlendUVDistortionUV;
    Input.FlipbookRate = in.Input_FlipbookRate;
    Input.FlipbookNextIndexUV = in.Input_FlipbookNextIndexUV;
    Input.AlphaThreshold = in.Input_AlphaThreshold;
    Input.Color = in.Input_Color;
    float4 _418 = _main(Input, g_uvDistortionTexture, g_uvDistortionSampler, v_133, g_colorTexture, g_colorSampler, g_alphaTexture, g_alphaSampler, g_blendUVDistortionTexture, g_blendUVDistortionSampler, g_blendTexture, g_blendSampler, g_blendAlphaTexture, g_blendAlphaSampler);
    out._entryPointOutput = _418;
    return out;
}

)";

