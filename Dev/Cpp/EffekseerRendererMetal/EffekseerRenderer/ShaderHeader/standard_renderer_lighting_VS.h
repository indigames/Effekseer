static const char metal_standard_renderer_lighting_VS[] = R"(mtlcode
#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VS_Input
{
    float3 Pos;
    float4 Color;
    float4 Normal;
    float4 Tangent;
    float2 UV1;
    float2 UV2;
    float2 AlphaUV;
    float2 UVDistortionUV;
    float2 BlendUV;
    float2 BlendAlphaUV;
    float2 BlendUVDistortionUV;
    float FlipbookIndex;
    float AlphaThreshold;
};

struct VS_Output
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

struct VS_ConstantBuffer
{
    float4x4 mCamera;
    float4x4 mProj;
    float4 mUVInversed;
    float4 mflipbookParameter;
};

struct main0_out
{
    float4 _entryPointOutput_VColor [[user(locn0)]];
    float2 _entryPointOutput_UV1 [[user(locn1)]];
    float2 _entryPointOutput_UV2 [[user(locn2)]];
    float3 _entryPointOutput_WorldP [[user(locn3)]];
    float3 _entryPointOutput_WorldN [[user(locn4)]];
    float3 _entryPointOutput_WorldT [[user(locn5)]];
    float3 _entryPointOutput_WorldB [[user(locn6)]];
    float2 _entryPointOutput_ScreenUV [[user(locn7)]];
    float2 _entryPointOutput_AlphaUV [[user(locn8)]];
    float2 _entryPointOutput_UVDistortionUV [[user(locn9)]];
    float2 _entryPointOutput_BlendUV [[user(locn10)]];
    float2 _entryPointOutput_BlendAlphaUV [[user(locn11)]];
    float2 _entryPointOutput_BlendUVDistortionUV [[user(locn12)]];
    float _entryPointOutput_FlipbookRate [[user(locn13)]];
    float2 _entryPointOutput_FlipbookNextIndexUV [[user(locn14)]];
    float _entryPointOutput_AlphaThreshold [[user(locn15)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float3 Input_Pos [[attribute(0)]];
    float4 Input_Color [[attribute(1)]];
    float4 Input_Normal [[attribute(2)]];
    float4 Input_Tangent [[attribute(3)]];
    float2 Input_UV1 [[attribute(4)]];
    float2 Input_UV2 [[attribute(5)]];
    float2 Input_AlphaUV [[attribute(6)]];
    float2 Input_UVDistortionUV [[attribute(7)]];
    float2 Input_BlendUV [[attribute(8)]];
    float2 Input_BlendAlphaUV [[attribute(9)]];
    float2 Input_BlendUVDistortionUV [[attribute(10)]];
    float Input_FlipbookIndex [[attribute(11)]];
    float Input_AlphaThreshold [[attribute(12)]];
};

// Implementation of the GLSL mod() function, which is slightly different than Metal fmod()
template<typename Tx, typename Ty>
inline Tx mod(Tx x, Ty y)
{
    return x - y * floor(x / y);
}

static inline __attribute__((always_inline))
float2 GetFlipbookOneSizeUV(thread const float& DivideX, thread const float& DivideY)
{
    return float2(1.0) / float2(DivideX, DivideY);
}

static inline __attribute__((always_inline))
float2 GetFlipbookOriginUV(thread const float2& FlipbookUV, thread const float& FlipbookIndex, thread const float& DivideX, thread const float& DivideY)
{
    float2 DivideIndex;
    DivideIndex.x = float(int(FlipbookIndex) % int(DivideX));
    DivideIndex.y = float(int(FlipbookIndex) / int(DivideX));
    float param = DivideX;
    float param_1 = DivideY;
    float2 FlipbookOneSize = GetFlipbookOneSizeUV(param, param_1);
    float2 UVOffset = DivideIndex * FlipbookOneSize;
    float2 OriginUV = FlipbookUV - UVOffset;
    OriginUV *= float2(DivideX, DivideY);
    return OriginUV;
}

static inline __attribute__((always_inline))
float2 GetFlipbookUVForIndex(thread const float2& OriginUV, thread const float& Index, thread const float& DivideX, thread const float& DivideY)
{
    float2 DivideIndex;
    DivideIndex.x = float(int(Index) % int(DivideX));
    DivideIndex.y = float(int(Index) / int(DivideX));
    float param = DivideX;
    float param_1 = DivideY;
    float2 FlipbookOneSize = GetFlipbookOneSizeUV(param, param_1);
    return (OriginUV * FlipbookOneSize) + (DivideIndex * FlipbookOneSize);
}

static inline __attribute__((always_inline))
void ApplyFlipbookVS(thread float& flipbookRate, thread float2& flipbookUV, thread const float4& flipbookParameter, thread const float& flipbookIndex, thread const float2& uv)
{
    if (flipbookParameter.x > 0.0)
    {
        flipbookRate = fract(flipbookIndex);
        float Index = floor(flipbookIndex);
        float IndexOffset = 1.0;
        float NextIndex = Index + IndexOffset;
        float FlipbookMaxCount = flipbookParameter.z * flipbookParameter.w;
        if (flipbookParameter.y == 0.0)
        {
            if (NextIndex >= FlipbookMaxCount)
            {
                NextIndex = FlipbookMaxCount - 1.0;
                Index = FlipbookMaxCount - 1.0;
            }
        }
        else
        {
            if (flipbookParameter.y == 1.0)
            {
                Index = mod(Index, FlipbookMaxCount);
                NextIndex = mod(NextIndex, FlipbookMaxCount);
            }
            else
            {
                if (flipbookParameter.y == 2.0)
                {
                    bool Reverse = mod(floor(Index / FlipbookMaxCount), 2.0) == 1.0;
                    Index = mod(Index, FlipbookMaxCount);
                    if (Reverse)
                    {
                        Index = (FlipbookMaxCount - 1.0) - floor(Index);
                    }
                    Reverse = mod(floor(NextIndex / FlipbookMaxCount), 2.0) == 1.0;
                    NextIndex = mod(NextIndex, FlipbookMaxCount);
                    if (Reverse)
                    {
                        NextIndex = (FlipbookMaxCount - 1.0) - floor(NextIndex);
                    }
                }
            }
        }
        float2 param = uv;
        float param_1 = Index;
        float param_2 = flipbookParameter.z;
        float param_3 = flipbookParameter.w;
        float2 OriginUV = GetFlipbookOriginUV(param, param_1, param_2, param_3);
        float2 param_4 = OriginUV;
        float param_5 = NextIndex;
        float param_6 = flipbookParameter.z;
        float param_7 = flipbookParameter.w;
        flipbookUV = GetFlipbookUVForIndex(param_4, param_5, param_6, param_7);
    }
}

static inline __attribute__((always_inline))
VS_Output _main(VS_Input Input, constant VS_ConstantBuffer& v_277)
{
    VS_Output Output = VS_Output{ float4(0.0), float4(0.0), float2(0.0), float2(0.0), float3(0.0), float3(0.0), float3(0.0), float3(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), 0.0, float2(0.0), 0.0 };
    float3 worldPos = Input.Pos;
    float3 worldNormal = (Input.Normal.xyz - float3(0.5)) * 2.0;
    float3 worldTangent = (Input.Tangent.xyz - float3(0.5)) * 2.0;
    float3 worldBinormal = cross(worldNormal, worldTangent);
    float2 uv1 = Input.UV1;
    float2 uv2 = Input.UV1;
    uv1.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * uv1.y);
    uv2.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * uv2.y);
    float2 alphaUV = Input.AlphaUV;
    alphaUV.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * alphaUV.y);
    float2 uvDistorionUV = Input.UVDistortionUV;
    uvDistorionUV.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * uvDistorionUV.y);
    float2 blendUV = Input.BlendUV;
    blendUV.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * blendUV.y);
    float2 blendAlphaUV = Input.BlendAlphaUV;
    blendAlphaUV.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * blendAlphaUV.y);
    float2 blendUVDistortionUV = Input.BlendUVDistortionUV;
    blendUVDistortionUV.y = v_277.mUVInversed.x + (v_277.mUVInversed.y * blendUVDistortionUV.y);
    float param = Output.FlipbookRate;
    float2 param_1 = Output.FlipbookNextIndexUV;
    float4 param_2 = v_277.mflipbookParameter;
    float param_3 = Input.FlipbookIndex;
    float2 param_4 = Output.UV1;
    ApplyFlipbookVS(param, param_1, param_2, param_3, param_4);
    Output.FlipbookRate = param;
    Output.FlipbookNextIndexUV = param_1;
    Output.AlphaThreshold = Input.AlphaThreshold;
    Output.WorldN = worldNormal;
    Output.WorldB = worldBinormal;
    Output.WorldT = worldTangent;
    float3 pixelNormalDir = float3(0.5, 0.5, 1.0);
    float4 cameraPos = v_277.mCamera * float4(worldPos, 1.0);
    cameraPos /= float4(cameraPos.w);
    Output.Position = v_277.mProj * cameraPos;
    Output.WorldP = worldPos;
    Output.VColor = Input.Color;
    Output.UV1 = uv1;
    Output.UV2 = uv2;
    Output.AlphaUV = alphaUV;
    Output.UVDistortionUV = uvDistorionUV;
    Output.BlendUV = blendUV;
    Output.BlendAlphaUV = blendAlphaUV;
    Output.BlendUVDistortionUV = blendUVDistortionUV;
    Output.ScreenUV = Output.Position.xy / float2(Output.Position.w);
    Output.ScreenUV = float2(Output.ScreenUV.x + 1.0, 1.0 - Output.ScreenUV.y) * 0.5;
    return Output;
}

vertex main0_out main0(main0_in in [[stage_in]], constant VS_ConstantBuffer& v_277 [[buffer(0)]])
{
    main0_out out = {};
    VS_Input Input;
    Input.Pos = in.Input_Pos;
    Input.Color = in.Input_Color;
    Input.Normal = in.Input_Normal;
    Input.Tangent = in.Input_Tangent;
    Input.UV1 = in.Input_UV1;
    Input.UV2 = in.Input_UV2;
    Input.AlphaUV = in.Input_AlphaUV;
    Input.UVDistortionUV = in.Input_UVDistortionUV;
    Input.BlendUV = in.Input_BlendUV;
    Input.BlendAlphaUV = in.Input_BlendAlphaUV;
    Input.BlendUVDistortionUV = in.Input_BlendUVDistortionUV;
    Input.FlipbookIndex = in.Input_FlipbookIndex;
    Input.AlphaThreshold = in.Input_AlphaThreshold;
    VS_Output flattenTemp = _main(Input, v_277);
    out.gl_Position = flattenTemp.Position;
    out._entryPointOutput_VColor = flattenTemp.VColor;
    out._entryPointOutput_UV1 = flattenTemp.UV1;
    out._entryPointOutput_UV2 = flattenTemp.UV2;
    out._entryPointOutput_WorldP = flattenTemp.WorldP;
    out._entryPointOutput_WorldN = flattenTemp.WorldN;
    out._entryPointOutput_WorldT = flattenTemp.WorldT;
    out._entryPointOutput_WorldB = flattenTemp.WorldB;
    out._entryPointOutput_ScreenUV = flattenTemp.ScreenUV;
    out._entryPointOutput_AlphaUV = flattenTemp.AlphaUV;
    out._entryPointOutput_UVDistortionUV = flattenTemp.UVDistortionUV;
    out._entryPointOutput_BlendUV = flattenTemp.BlendUV;
    out._entryPointOutput_BlendAlphaUV = flattenTemp.BlendAlphaUV;
    out._entryPointOutput_BlendUVDistortionUV = flattenTemp.BlendUVDistortionUV;
    out._entryPointOutput_FlipbookRate = flattenTemp.FlipbookRate;
    out._entryPointOutput_FlipbookNextIndexUV = flattenTemp.FlipbookNextIndexUV;
    out._entryPointOutput_AlphaThreshold = flattenTemp.AlphaThreshold;
    return out;
}

)";

