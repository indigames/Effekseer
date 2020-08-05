struct PS_Input
{
    float4 Pos;
    float4 Color;
    float2 UV;
    float4 Position;
    float4 PosU;
    float4 PosR;
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

cbuffer PS_ConstanBuffer : register(b1)
{
    float4 _186_flipbookParameter : packoffset(c0);
    float4 _186_uvDistortionParameter : packoffset(c1);
    float4 _186_blendTextureParameter : packoffset(c2);
    float4 _186_emissiveScaling : packoffset(c3);
    float4 _186_edgeColor : packoffset(c4);
    float4 _186_edgeParameter : packoffset(c5);
};

Texture2D<float4> g_uvDistortionTexture : register(t2);
SamplerState g_uvDistortionSampler : register(s2);
Texture2D<float4> g_texture : register(t0);
SamplerState g_sampler : register(s0);
Texture2D<float4> g_alphaTexture : register(t1);
SamplerState g_alphaSampler : register(s1);
Texture2D<float4> g_blendUVDistortionTexture : register(t5);
SamplerState g_blendUVDistortionSampler : register(s5);
Texture2D<float4> g_blendTexture : register(t3);
SamplerState g_blendSampler : register(s3);
Texture2D<float4> g_blendAlphaTexture : register(t4);
SamplerState g_blendAlphaSampler : register(s4);

static float4 gl_FragCoord;
static float4 Input_Color;
static float2 Input_UV;
static float4 Input_Position;
static float4 Input_PosU;
static float4 Input_PosR;
static float4 Input_Alpha_Dist_UV;
static float4 Input_Blend_Alpha_Dist_UV;
static float4 Input_Blend_FBNextIndex_UV;
static float2 Input_Others;
static float4 _entryPointOutput;

struct SPIRV_Cross_Input
{
    float4 Input_Color : TEXCOORD0;
    float2 Input_UV : TEXCOORD1;
    float4 Input_Position : TEXCOORD2;
    float4 Input_PosU : TEXCOORD3;
    float4 Input_PosR : TEXCOORD4;
    float4 Input_Alpha_Dist_UV : TEXCOORD5;
    float4 Input_Blend_Alpha_Dist_UV : TEXCOORD6;
    float4 Input_Blend_FBNextIndex_UV : TEXCOORD7;
    float2 Input_Others : TEXCOORD8;
    float4 gl_FragCoord : SV_Position;
};

struct SPIRV_Cross_Output
{
    float4 _entryPointOutput : SV_Target0;
};

AdvancedParameter DisolveAdvancedParameter(PS_Input psinput)
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

void ApplyFlipbook(inout float4 dst, Texture2D<float4> t, SamplerState s, float4 flipbookParameter, float4 vcolor, float2 nextUV, float flipbookRate)
{
    if (flipbookParameter.x > 0.0f)
    {
        float4 NextPixelColor = t.Sample(s, nextUV) * vcolor;
        if (flipbookParameter.y == 1.0f)
        {
            dst = lerp(dst, NextPixelColor, flipbookRate.xxxx);
        }
    }
}

void ApplyTextureBlending(inout float4 dstColor, float4 blendColor, float blendType)
{
    if (blendType == 0.0f)
    {
        float3 _77 = (blendColor.xyz * blendColor.w) + (dstColor.xyz * (1.0f - blendColor.w));
        dstColor = float4(_77.x, _77.y, _77.z, dstColor.w);
    }
    else
    {
        if (blendType == 1.0f)
        {
            float3 _89 = dstColor.xyz + (blendColor.xyz * blendColor.w);
            dstColor = float4(_89.x, _89.y, _89.z, dstColor.w);
        }
        else
        {
            if (blendType == 2.0f)
            {
                float3 _102 = dstColor.xyz - (blendColor.xyz * blendColor.w);
                dstColor = float4(_102.x, _102.y, _102.z, dstColor.w);
            }
            else
            {
                if (blendType == 3.0f)
                {
                    float3 _115 = dstColor.xyz * (blendColor.xyz * blendColor.w);
                    dstColor = float4(_115.x, _115.y, _115.z, dstColor.w);
                }
            }
        }
    }
}

float4 _main(PS_Input Input)
{
    PS_Input param = Input;
    AdvancedParameter advancedParam = DisolveAdvancedParameter(param);
    float2 UVOffset = 0.0f.xx;
    UVOffset = (g_uvDistortionTexture.Sample(g_uvDistortionSampler, advancedParam.AlphaUV).xy * 2.0f) - 1.0f.xx;
    UVOffset *= _186_uvDistortionParameter.x;
    float4 Output = Input.Color * g_texture.Sample(g_sampler, Input.UV + UVOffset);
    float4 param_1 = Output;
    float param_2 = advancedParam.FlipbookRate;
    ApplyFlipbook(param_1, g_texture, g_sampler, _186_flipbookParameter, Input.Color, advancedParam.FlipbookNextIndexUV + UVOffset, param_2);
    Output = param_1;
    float4 AlphaTexColor = g_alphaTexture.Sample(g_alphaSampler, advancedParam.UVDistortionUV + UVOffset);
    Output.w *= (AlphaTexColor.x * AlphaTexColor.w);
    float2 BlendUVOffset = (g_blendUVDistortionTexture.Sample(g_blendUVDistortionSampler, advancedParam.BlendUVDistortionUV).xy * 2.0f) - 1.0f.xx;
    BlendUVOffset *= _186_uvDistortionParameter.y;
    float4 BlendTextureColor = g_blendTexture.Sample(g_blendSampler, advancedParam.BlendUV + BlendUVOffset);
    float4 BlendAlphaTextureColor = g_blendAlphaTexture.Sample(g_blendAlphaSampler, advancedParam.BlendAlphaUV + BlendUVOffset);
    BlendTextureColor.w *= (BlendAlphaTextureColor.x * BlendAlphaTextureColor.w);
    float4 param_3 = Output;
    ApplyTextureBlending(param_3, BlendTextureColor, _186_blendTextureParameter.x);
    Output = param_3;
    float3 _300 = Output.xyz * _186_emissiveScaling.x;
    Output = float4(_300.x, _300.y, _300.z, Output.w);
    if (Output.w <= max(0.0f, advancedParam.AlphaThreshold))
    {
        discard;
    }
    float3 _330 = lerp(_186_edgeColor.xyz * _186_edgeParameter.y, Output.xyz, ceil((Output.w - advancedParam.AlphaThreshold) - _186_edgeParameter.x).xxx);
    Output = float4(_330.x, _330.y, _330.z, Output.w);
    return Output;
}

void frag_main()
{
    PS_Input Input;
    Input.Pos = gl_FragCoord;
    Input.Color = Input_Color;
    Input.UV = Input_UV;
    Input.Position = Input_Position;
    Input.PosU = Input_PosU;
    Input.PosR = Input_PosR;
    Input.Alpha_Dist_UV = Input_Alpha_Dist_UV;
    Input.Blend_Alpha_Dist_UV = Input_Blend_Alpha_Dist_UV;
    Input.Blend_FBNextIndex_UV = Input_Blend_FBNextIndex_UV;
    Input.Others = Input_Others;
    float4 _372 = _main(Input);
    _entryPointOutput = _372;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    gl_FragCoord = stage_input.gl_FragCoord;
    Input_Color = stage_input.Input_Color;
    Input_UV = stage_input.Input_UV;
    Input_Position = stage_input.Input_Position;
    Input_PosU = stage_input.Input_PosU;
    Input_PosR = stage_input.Input_PosR;
    Input_Alpha_Dist_UV = stage_input.Input_Alpha_Dist_UV;
    Input_Blend_Alpha_Dist_UV = stage_input.Input_Blend_Alpha_Dist_UV;
    Input_Blend_FBNextIndex_UV = stage_input.Input_Blend_FBNextIndex_UV;
    Input_Others = stage_input.Input_Others;
    frag_main();
    SPIRV_Cross_Output stage_output;
    stage_output._entryPointOutput = _entryPointOutput;
    return stage_output;
}
