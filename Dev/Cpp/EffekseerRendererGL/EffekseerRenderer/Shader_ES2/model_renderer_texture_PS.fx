#version 200 es
precision mediump float;
precision highp int;

struct PS_Input
{
    highp vec4 Pos;
    highp vec2 UV;
    highp vec3 Normal;
    highp vec3 Binormal;
    highp vec3 Tangent;
    highp vec4 Color;
    highp vec4 Alpha_Dist_UV;
    highp vec4 Blend_Alpha_Dist_UV;
    highp vec4 Blend_FBNextIndex_UV;
    highp vec2 Others;
};

struct AdvancedParameter
{
    highp vec2 AlphaUV;
    highp vec2 UVDistortionUV;
    highp vec2 BlendUV;
    highp vec2 BlendAlphaUV;
    highp vec2 BlendUVDistortionUV;
    highp vec2 FlipbookNextIndexUV;
    highp float FlipbookRate;
    highp float AlphaThreshold;
};

struct FalloffParameter
{
    highp vec4 Param;
    highp vec4 BeginColor;
    highp vec4 EndColor;
};

struct PS_ConstanBuffer
{
    highp vec4 fLightDirection;
    highp vec4 fLightColor;
    highp vec4 fLightAmbient;
    highp vec4 fFlipbookParameter;
    highp vec4 fUVDistortionParameter;
    highp vec4 fBlendTextureParameter;
    highp vec4 fCameraFrontDirection;
    FalloffParameter fFalloffParam;
    highp vec4 fEmissiveScaling;
    highp vec4 fEdgeColor;
    highp vec4 fEdgeParameter;
};

uniform PS_ConstanBuffer CBPS0;

uniform highp sampler2D Sampler_g_uvDistortionSampler;
uniform highp sampler2D Sampler_g_colorSampler;
uniform highp sampler2D Sampler_g_alphaSampler;
uniform highp sampler2D Sampler_g_blendUVDistortionSampler;
uniform highp sampler2D Sampler_g_blendSampler;
uniform highp sampler2D Sampler_g_blendAlphaSampler;

varying highp vec2 _VSPS_UV;
varying highp vec3 _VSPS_Normal;
varying highp vec3 _VSPS_Binormal;
varying highp vec3 _VSPS_Tangent;
varying highp vec4 _VSPS_Color;
varying highp vec4 _VSPS_Alpha_Dist_UV;
varying highp vec4 _VSPS_Blend_Alpha_Dist_UV;
varying highp vec4 _VSPS_Blend_FBNextIndex_UV;
varying highp vec2 _VSPS_Others;

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

void ApplyFlipbook(inout highp vec4 dst, highp vec4 flipbookParameter, highp vec4 vcolor, highp vec2 nextUV, highp float flipbookRate, highp sampler2D SPIRV_Cross_Combinedts)
{
    if (flipbookParameter.x > 0.0)
    {
        highp vec4 NextPixelColor = texture2D(SPIRV_Cross_Combinedts, nextUV) * vcolor;
        if (flipbookParameter.y == 1.0)
        {
            dst = mix(dst, NextPixelColor, vec4(flipbookRate));
        }
    }
}

void ApplyTextureBlending(inout highp vec4 dstColor, highp vec4 blendColor, highp float blendType)
{
    if (blendType == 0.0)
    {
        highp vec3 _77 = (blendColor.xyz * blendColor.w) + (dstColor.xyz * (1.0 - blendColor.w));
        dstColor = vec4(_77.x, _77.y, _77.z, dstColor.w);
    }
    else
    {
        if (blendType == 1.0)
        {
            highp vec3 _89 = dstColor.xyz + (blendColor.xyz * blendColor.w);
            dstColor = vec4(_89.x, _89.y, _89.z, dstColor.w);
        }
        else
        {
            if (blendType == 2.0)
            {
                highp vec3 _102 = dstColor.xyz - (blendColor.xyz * blendColor.w);
                dstColor = vec4(_102.x, _102.y, _102.z, dstColor.w);
            }
            else
            {
                if (blendType == 3.0)
                {
                    highp vec3 _115 = dstColor.xyz * (blendColor.xyz * blendColor.w);
                    dstColor = vec4(_115.x, _115.y, _115.z, dstColor.w);
                }
            }
        }
    }
}

highp vec4 _main(PS_Input Input)
{
    PS_Input param = Input;
    AdvancedParameter advancedParam = DisolveAdvancedParameter(param);
    highp vec2 UVOffset = vec2(0.0);
    UVOffset = (texture2D(Sampler_g_uvDistortionSampler, advancedParam.UVDistortionUV).xy * 2.0) - vec2(1.0);
    UVOffset *= CBPS0.fUVDistortionParameter.x;
    highp vec4 Output = texture2D(Sampler_g_colorSampler, Input.UV + UVOffset) * Input.Color;
    highp vec4 param_1 = Output;
    highp float param_2 = advancedParam.FlipbookRate;
    ApplyFlipbook(param_1, CBPS0.fFlipbookParameter, Input.Color, advancedParam.FlipbookNextIndexUV + UVOffset, param_2, Sampler_g_colorSampler);
    Output = param_1;
    highp vec4 AlphaTexColor = texture2D(Sampler_g_alphaSampler, advancedParam.AlphaUV + UVOffset);
    Output.w *= (AlphaTexColor.x * AlphaTexColor.w);
    highp vec2 BlendUVOffset = (texture2D(Sampler_g_blendUVDistortionSampler, advancedParam.BlendUVDistortionUV).xy * 2.0) - vec2(1.0);
    BlendUVOffset *= CBPS0.fUVDistortionParameter.y;
    highp vec4 BlendTextureColor = texture2D(Sampler_g_blendSampler, advancedParam.BlendUV + BlendUVOffset);
    highp vec4 BlendAlphaTextureColor = texture2D(Sampler_g_blendAlphaSampler, advancedParam.BlendAlphaUV + BlendUVOffset);
    BlendTextureColor.w *= (BlendAlphaTextureColor.x * BlendAlphaTextureColor.w);
    highp vec4 param_3 = Output;
    ApplyTextureBlending(param_3, BlendTextureColor, CBPS0.fBlendTextureParameter.x);
    Output = param_3;
    if (CBPS0.fFalloffParam.Param.x == 1.0)
    {
        highp vec3 cameraVec = normalize(-CBPS0.fCameraFrontDirection.xyz);
        highp float CdotN = clamp(dot(cameraVec, normalize(Input.Normal)), 0.0, 1.0);
        highp vec4 FalloffBlendColor = mix(CBPS0.fFalloffParam.EndColor, CBPS0.fFalloffParam.BeginColor, vec4(pow(CdotN, CBPS0.fFalloffParam.Param.z)));
        if (CBPS0.fFalloffParam.Param.y == 0.0)
        {
            highp vec3 _336 = Output.xyz + FalloffBlendColor.xyz;
            Output = vec4(_336.x, _336.y, _336.z, Output.w);
        }
        else
        {
            if (CBPS0.fFalloffParam.Param.y == 1.0)
            {
                highp vec3 _349 = Output.xyz - FalloffBlendColor.xyz;
                Output = vec4(_349.x, _349.y, _349.z, Output.w);
            }
            else
            {
                if (CBPS0.fFalloffParam.Param.y == 2.0)
                {
                    highp vec3 _362 = Output.xyz * FalloffBlendColor.xyz;
                    Output = vec4(_362.x, _362.y, _362.z, Output.w);
                }
            }
        }
        Output.w *= FalloffBlendColor.w;
    }
    highp vec3 _375 = Output.xyz * CBPS0.fEmissiveScaling.x;
    Output = vec4(_375.x, _375.y, _375.z, Output.w);
    if (Output.w <= max(0.0, advancedParam.AlphaThreshold))
    {
        discard;
    }
    highp vec3 _406 = mix(CBPS0.fEdgeColor.xyz * CBPS0.fEdgeParameter.y, Output.xyz, vec3(ceil((Output.w - advancedParam.AlphaThreshold) - CBPS0.fEdgeParameter.x)));
    Output = vec4(_406.x, _406.y, _406.z, Output.w);
    return Output;
}

void main()
{
    PS_Input Input;
    Input.Pos = gl_FragCoord;
    Input.UV = _VSPS_UV;
    Input.Normal = _VSPS_Normal;
    Input.Binormal = _VSPS_Binormal;
    Input.Tangent = _VSPS_Tangent;
    Input.Color = _VSPS_Color;
    Input.Alpha_Dist_UV = _VSPS_Alpha_Dist_UV;
    Input.Blend_Alpha_Dist_UV = _VSPS_Blend_Alpha_Dist_UV;
    Input.Blend_FBNextIndex_UV = _VSPS_Blend_FBNextIndex_UV;
    Input.Others = _VSPS_Others;
    highp vec4 _449 = _main(Input);
    gl_FragData[0] = _449;
}

