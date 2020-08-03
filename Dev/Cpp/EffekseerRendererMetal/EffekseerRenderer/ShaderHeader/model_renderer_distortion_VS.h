static const char metal_model_renderer_distortion_VS[] = R"(mtlcode
#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VS_Input
{
    float3 Pos;
    float3 Normal;
    float3 Binormal;
    float3 Tangent;
    float2 UV;
    uint4 Index;
};

struct VS_Output
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

struct VS_ConstantBuffer
{
    float4x4 mCameraProj;
    float4x4 mModel[1];
    float4 fUV[1];
    float4 fAlphaUV[1];
    float4 fUVDistortionUV[1];
    float4 fBlendUV[1];
    float4 fBlendAlphaUV[1];
    float4 fBlendUVDistortionUV[1];
    float4 fFlipbookParameter;
    float4 fFlipbookIndexAndNextRate[1];
    float4 fModelAlphaThreshold[1];
    float4 fModelColor[1];
    float4 fLightDirection;
    float4 fLightColor;
    float4 fLightAmbient;
    float4 mUVInversed;
};

struct main0_out
{
    float2 _entryPointOutput_UV [[user(locn0)]];
    float4 _entryPointOutput_Normal [[user(locn1)]];
    float4 _entryPointOutput_Binormal [[user(locn2)]];
    float4 _entryPointOutput_Tangent [[user(locn3)]];
    float4 _entryPointOutput_Pos [[user(locn4)]];
    float4 _entryPointOutput_Color [[user(locn5)]];
    float2 _entryPointOutput_AlphaUV [[user(locn6)]];
    float2 _entryPointOutput_UVDistortionUV [[user(locn7)]];
    float2 _entryPointOutput_BlendUV [[user(locn8)]];
    float2 _entryPointOutput_BlendAlphaUV [[user(locn9)]];
    float2 _entryPointOutput_BlendUVDistortionUV [[user(locn10)]];
    float _entryPointOutput_FlipbookRate [[user(locn11)]];
    float2 _entryPointOutput_FlipbookNextIndexUV [[user(locn12)]];
    float _entryPointOutput_AlphaThreshold [[user(locn13)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float3 Input_Pos [[attribute(0)]];
    float3 Input_Normal [[attribute(1)]];
    float3 Input_Binormal [[attribute(2)]];
    float3 Input_Tangent [[attribute(3)]];
    float2 Input_UV [[attribute(4)]];
    uint4 Input_Index [[attribute(5)]];
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
VS_Output _main(VS_Input Input, constant VS_ConstantBuffer& v_257)
{
    float4x4 matModel = transpose(v_257.mModel[Input.Index.x]);
    float4 uv = v_257.fUV[Input.Index.x];
    float4 modelColor = v_257.fModelColor[Input.Index.x];
    float4 alphaUV = v_257.fAlphaUV[Input.Index.x];
    float4 uvDistortionUV = v_257.fUVDistortionUV[Input.Index.x];
    float4 blendUV = v_257.fBlendUV[Input.Index.x];
    float4 blendAlphaUV = v_257.fBlendAlphaUV[Input.Index.x];
    float4 blendUVDistortionUV = v_257.fBlendUVDistortionUV[Input.Index.x];
    float flipbookIndexAndNextRate = v_257.fFlipbookIndexAndNextRate[Input.Index.x].x;
    float modelAlphaThreshold = v_257.fModelAlphaThreshold[Input.Index.x].x;
    VS_Output Output = VS_Output{ float4(0.0), float2(0.0), float4(0.0), float4(0.0), float4(0.0), float4(0.0), float4(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), 0.0, float2(0.0), 0.0 };
    float4 localPosition = float4(Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0);
    float4 localNormal = float4(Input.Pos.x + Input.Normal.x, Input.Pos.y + Input.Normal.y, Input.Pos.z + Input.Normal.z, 1.0);
    float4 localBinormal = float4(Input.Pos.x + Input.Binormal.x, Input.Pos.y + Input.Binormal.y, Input.Pos.z + Input.Binormal.z, 1.0);
    float4 localTangent = float4(Input.Pos.x + Input.Tangent.x, Input.Pos.y + Input.Tangent.y, Input.Pos.z + Input.Tangent.z, 1.0);
    localPosition *= matModel;
    localNormal *= matModel;
    localBinormal *= matModel;
    localTangent *= matModel;
    localNormal = localPosition + normalize(localNormal - localPosition);
    localBinormal = localPosition + normalize(localBinormal - localPosition);
    localTangent = localPosition + normalize(localTangent - localPosition);
    Output.Position = v_257.mCameraProj * localPosition;
    Output.UV.x = (Input.UV.x * uv.z) + uv.x;
    Output.UV.y = (Input.UV.y * uv.w) + uv.y;
    Output.AlphaUV.x = (Input.UV.x * alphaUV.z) + alphaUV.x;
    Output.AlphaUV.y = (Input.UV.y * alphaUV.w) + alphaUV.y;
    Output.UVDistortionUV.x = (Input.UV.x * uvDistortionUV.z) + uvDistortionUV.x;
    Output.UVDistortionUV.y = (Input.UV.y * uvDistortionUV.w) + uvDistortionUV.y;
    Output.BlendUV.x = (Input.UV.x * blendUV.z) + blendUV.x;
    Output.BlendUV.y = (Input.UV.y * blendUV.w) + blendUV.y;
    Output.BlendAlphaUV.x = (Input.UV.x * blendAlphaUV.z) + blendAlphaUV.x;
    Output.BlendAlphaUV.y = (Input.UV.y * blendAlphaUV.w) + blendAlphaUV.y;
    Output.BlendUVDistortionUV.x = (Input.UV.x * blendUVDistortionUV.z) + blendUVDistortionUV.x;
    Output.BlendUVDistortionUV.y = (Input.UV.y * blendUVDistortionUV.w) + blendUVDistortionUV.y;
    Output.Normal = v_257.mCameraProj * localNormal;
    Output.Binormal = v_257.mCameraProj * localBinormal;
    Output.Tangent = v_257.mCameraProj * localTangent;
    Output.Pos = Output.Position;
    Output.Color = modelColor;
    Output.UV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Output.UV.y);
    Output.AlphaUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Output.AlphaUV.y);
    Output.UVDistortionUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Output.UVDistortionUV.y);
    Output.BlendUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Output.BlendUV.y);
    Output.BlendAlphaUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Output.BlendAlphaUV.y);
    Output.BlendUVDistortionUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Output.BlendUVDistortionUV.y);
    float param = Output.FlipbookRate;
    float2 param_1 = Output.FlipbookNextIndexUV;
    float4 param_2 = v_257.fFlipbookParameter;
    float param_3 = flipbookIndexAndNextRate;
    float2 param_4 = Output.UV;
    ApplyFlipbookVS(param, param_1, param_2, param_3, param_4);
    Output.FlipbookRate = param;
    Output.FlipbookNextIndexUV = param_1;
    Output.AlphaThreshold = modelAlphaThreshold;
    return Output;
}

vertex main0_out main0(main0_in in [[stage_in]], constant VS_ConstantBuffer& v_257 [[buffer(0)]])
{
    main0_out out = {};
    VS_Input Input;
    Input.Pos = in.Input_Pos;
    Input.Normal = in.Input_Normal;
    Input.Binormal = in.Input_Binormal;
    Input.Tangent = in.Input_Tangent;
    Input.UV = in.Input_UV;
    Input.Index = in.Input_Index;
    VS_Output flattenTemp = _main(Input, v_257);
    out.gl_Position = flattenTemp.Position;
    out._entryPointOutput_UV = flattenTemp.UV;
    out._entryPointOutput_Normal = flattenTemp.Normal;
    out._entryPointOutput_Binormal = flattenTemp.Binormal;
    out._entryPointOutput_Tangent = flattenTemp.Tangent;
    out._entryPointOutput_Pos = flattenTemp.Pos;
    out._entryPointOutput_Color = flattenTemp.Color;
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

