static const char metal_model_renderer_distortion_PS[] = R"(mtlcode
#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct PS_Input
{
    float4 Position;
    float2 UV;
    float4 Normal;
    float4 Binormal;
    float4 Tangent;
    float4 Pos;
    float4 Color;
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
    float4 g_scale;
    float4 mUVInversedBack;
    float4 fFlipbookParameter;
    float4 fUVDistortionParameter;
    float4 fBlendTextureParameter;
};

struct main0_out
{
    float4 _entryPointOutput [[color(0)]];
};

struct main0_in
{
    float2 Input_UV [[user(locn0)]];
    float4 Input_Normal [[user(locn1)]];
    float4 Input_Binormal [[user(locn2)]];
    float4 Input_Tangent [[user(locn3)]];
    float4 Input_Pos [[user(locn4)]];
    float4 Input_Color [[user(locn5)]];
    float2 Input_AlphaUV [[user(locn6)]];
    float2 Input_UVDistortionUV [[user(locn7)]];
    float2 Input_BlendUV [[user(locn8)]];
    float2 Input_BlendAlphaUV [[user(locn9)]];
    float2 Input_BlendUVDistortionUV [[user(locn10)]];
    float Input_FlipbookRate [[user(locn11)]];
    float2 Input_FlipbookNextIndexUV [[user(locn12)]];
    float Input_AlphaThreshold [[user(locn13)]];
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
float4 _main(PS_Input Input, thread texture2d<float> g_uvDistortionTexture, thread sampler g_uvDistortionSampler, constant PS_ConstanBuffer& v_132, thread texture2d<float> g_texture, thread sampler g_sampler, thread texture2d<float> g_alphaTexture, thread sampler g_alphaSampler, thread texture2d<float> g_blendUVDistortionTexture, thread sampler g_blendUVDistortionSampler, thread texture2d<float> g_blendTexture, thread sampler g_blendSampler, thread texture2d<float> g_blendAlphaTexture, thread sampler g_blendAlphaSampler, thread texture2d<float> g_backTexture, thread sampler g_backSampler)
{
    float2 UVOffset = float2(0.0);
    UVOffset = (g_uvDistortionTexture.sample(g_uvDistortionSampler, Input.UVDistortionUV).xy * 2.0) - float2(1.0);
    UVOffset *= v_132.fUVDistortionParameter.x;
    float4 Output = g_texture.sample(g_sampler, (Input.UV + UVOffset));
    Output.w *= Input.Color.w;
    float4 param = Output;
    float param_1 = Input.FlipbookRate;
    ApplyFlipbook(param, g_texture, g_sampler, v_132.fFlipbookParameter, Input.Color, Input.FlipbookNextIndexUV + UVOffset, param_1);
    Output = param;
    float4 AlphaTexColor = g_alphaTexture.sample(g_alphaSampler, (Input.AlphaUV + UVOffset));
    Output.w *= (AlphaTexColor.x * AlphaTexColor.w);
    float2 BlendUVOffset = (g_blendUVDistortionTexture.sample(g_blendUVDistortionSampler, Input.BlendUVDistortionUV).xy * 2.0) - float2(1.0);
    BlendUVOffset *= v_132.fUVDistortionParameter.y;
    float4 BlendTextureColor = g_blendTexture.sample(g_blendSampler, (Input.BlendUV + BlendUVOffset));
    float4 BlendAlphaTextureColor = g_blendAlphaTexture.sample(g_blendAlphaSampler, (Input.BlendAlphaUV + BlendUVOffset));
    BlendTextureColor.w *= (BlendAlphaTextureColor.x * BlendAlphaTextureColor.w);
    float4 param_2 = Output;
    ApplyTextureBlending(param_2, BlendTextureColor, v_132.fBlendTextureParameter.x);
    Output = param_2;
    if (Output.w <= Input.AlphaThreshold)
    {
        discard_fragment();
    }
    if (Output.w == 0.0)
    {
        discard_fragment();
    }
    float2 pos = Input.Pos.xy / float2(Input.Pos.w);
    float2 posU = Input.Tangent.xy / float2(Input.Tangent.w);
    float2 posR = Input.Binormal.xy / float2(Input.Binormal.w);
    float xscale = (((Output.x * 2.0) - 1.0) * Input.Color.x) * v_132.g_scale.x;
    float yscale = (((Output.y * 2.0) - 1.0) * Input.Color.y) * v_132.g_scale.x;
    float2 uv = (pos + ((posR - pos) * xscale)) + ((posU - pos) * yscale);
    uv.x = (uv.x + 1.0) * 0.5;
    uv.y = 1.0 - ((uv.y + 1.0) * 0.5);
    uv.y = v_132.mUVInversedBack.x + (v_132.mUVInversedBack.y * uv.y);
    float3 color = float3(g_backTexture.sample(g_backSampler, uv).xyz);
    Output = float4(color.x, color.y, color.z, Output.w);
    return Output;
}

fragment main0_out main0(main0_in in [[stage_in]], constant PS_ConstanBuffer& v_132 [[buffer(0)]], texture2d<float> g_uvDistortionTexture [[texture(0)]], texture2d<float> g_texture [[texture(1)]], texture2d<float> g_alphaTexture [[texture(2)]], texture2d<float> g_blendUVDistortionTexture [[texture(3)]], texture2d<float> g_blendTexture [[texture(4)]], texture2d<float> g_blendAlphaTexture [[texture(5)]], texture2d<float> g_backTexture [[texture(6)]], sampler g_uvDistortionSampler [[sampler(0)]], sampler g_sampler [[sampler(1)]], sampler g_alphaSampler [[sampler(2)]], sampler g_blendUVDistortionSampler [[sampler(3)]], sampler g_blendSampler [[sampler(4)]], sampler g_blendAlphaSampler [[sampler(5)]], sampler g_backSampler [[sampler(6)]], float4 gl_FragCoord [[position]])
{
    main0_out out = {};
    PS_Input Input;
    Input.Position = gl_FragCoord;
    Input.UV = in.Input_UV;
    Input.Normal = in.Input_Normal;
    Input.Binormal = in.Input_Binormal;
    Input.Tangent = in.Input_Tangent;
    Input.Pos = in.Input_Pos;
    Input.Color = in.Input_Color;
    Input.AlphaUV = in.Input_AlphaUV;
    Input.UVDistortionUV = in.Input_UVDistortionUV;
    Input.BlendUV = in.Input_BlendUV;
    Input.BlendAlphaUV = in.Input_BlendAlphaUV;
    Input.BlendUVDistortionUV = in.Input_BlendUVDistortionUV;
    Input.FlipbookRate = in.Input_FlipbookRate;
    Input.FlipbookNextIndexUV = in.Input_FlipbookNextIndexUV;
    Input.AlphaThreshold = in.Input_AlphaThreshold;
    float4 _411 = _main(Input, g_uvDistortionTexture, g_uvDistortionSampler, v_132, g_texture, g_sampler, g_alphaTexture, g_alphaSampler, g_blendUVDistortionTexture, g_blendUVDistortionSampler, g_blendTexture, g_blendSampler, g_blendAlphaTexture, g_blendAlphaSampler, g_backTexture, g_backSampler);
    out._entryPointOutput = _411;
    return out;
}

)";

