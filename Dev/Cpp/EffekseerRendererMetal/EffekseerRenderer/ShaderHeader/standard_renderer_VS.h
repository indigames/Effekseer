static const char metal_standard_renderer_VS[] = R"(mtlcode
#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct VS_Input
{
    float3 Pos;
    float4 Color;
    float2 UV;
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
    float4 Color;
    float2 UV;
    float4 Pos;
    float4 PosU;
    float4 PosR;
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
    float4 _entryPointOutput_Color [[user(locn0)]];
    float2 _entryPointOutput_UV [[user(locn1)]];
    float4 _entryPointOutput_Pos [[user(locn2)]];
    float4 _entryPointOutput_PosU [[user(locn3)]];
    float4 _entryPointOutput_PosR [[user(locn4)]];
    float2 _entryPointOutput_AlphaUV [[user(locn5)]];
    float2 _entryPointOutput_UVDistortionUV [[user(locn6)]];
    float2 _entryPointOutput_BlendUV [[user(locn7)]];
    float2 _entryPointOutput_BlendAlphaUV [[user(locn8)]];
    float2 _entryPointOutput_BlendUVDistortionUV [[user(locn9)]];
    float _entryPointOutput_FlipbookRate [[user(locn10)]];
    float2 _entryPointOutput_FlipbookNextIndexUV [[user(locn11)]];
    float _entryPointOutput_AlphaThreshold [[user(locn12)]];
    float4 gl_Position [[position]];
};

struct main0_in
{
    float3 Input_Pos [[attribute(0)]];
    float4 Input_Color [[attribute(1)]];
    float2 Input_UV [[attribute(2)]];
    float2 Input_AlphaUV [[attribute(3)]];
    float2 Input_UVDistortionUV [[attribute(4)]];
    float2 Input_BlendUV [[attribute(5)]];
    float2 Input_BlendAlphaUV [[attribute(6)]];
    float2 Input_BlendUVDistortionUV [[attribute(7)]];
    float Input_FlipbookIndex [[attribute(8)]];
    float Input_AlphaThreshold [[attribute(9)]];
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
    VS_Output Output = VS_Output{ float4(0.0), float4(0.0), float2(0.0), float4(0.0), float4(0.0), float4(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), float2(0.0), 0.0, float2(0.0), 0.0 };
    float4 pos4 = float4(Input.Pos.x, Input.Pos.y, Input.Pos.z, 1.0);
    float4 cameraPos = v_257.mCamera * pos4;
    cameraPos /= float4(cameraPos.w);
    Output.Position = v_257.mProj * cameraPos;
    Output.Pos = Output.Position;
    float4 cameraPosU = cameraPos + float4(0.0, 1.0, 0.0, 0.0);
    float4 cameraPosR = cameraPos + float4(1.0, 0.0, 0.0, 0.0);
    Output.PosU = v_257.mProj * cameraPosU;
    Output.PosR = v_257.mProj * cameraPosR;
    Output.PosU /= float4(Output.PosU.w);
    Output.PosR /= float4(Output.PosR.w);
    Output.Pos /= float4(Output.Pos.w);
    Output.Color = Input.Color;
    Output.UV = Input.UV;
    Output.UV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Input.UV.y);
    Output.AlphaUV = Input.AlphaUV;
    Output.AlphaUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Input.AlphaUV.y);
    Output.UVDistortionUV = Input.UVDistortionUV;
    Output.UVDistortionUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Input.UVDistortionUV.y);
    Output.BlendUV = Input.BlendUV;
    Output.BlendUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Input.BlendUV.y);
    Output.BlendAlphaUV = Input.BlendAlphaUV;
    Output.BlendAlphaUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Input.BlendAlphaUV.y);
    Output.BlendUVDistortionUV = Input.BlendUVDistortionUV;
    Output.BlendUVDistortionUV.y = v_257.mUVInversed.x + (v_257.mUVInversed.y * Input.BlendUVDistortionUV.y);
    float param = Output.FlipbookRate;
    float2 param_1 = Output.FlipbookNextIndexUV;
    float4 param_2 = v_257.mflipbookParameter;
    float param_3 = Input.FlipbookIndex;
    float2 param_4 = Output.UV;
    ApplyFlipbookVS(param, param_1, param_2, param_3, param_4);
    Output.FlipbookRate = param;
    Output.FlipbookNextIndexUV = param_1;
    Output.AlphaThreshold = Input.AlphaThreshold;
    return Output;
}

vertex main0_out main0(main0_in in [[stage_in]], constant VS_ConstantBuffer& v_257 [[buffer(0)]])
{
    main0_out out = {};
    VS_Input Input;
    Input.Pos = in.Input_Pos;
    Input.Color = in.Input_Color;
    Input.UV = in.Input_UV;
    Input.AlphaUV = in.Input_AlphaUV;
    Input.UVDistortionUV = in.Input_UVDistortionUV;
    Input.BlendUV = in.Input_BlendUV;
    Input.BlendAlphaUV = in.Input_BlendAlphaUV;
    Input.BlendUVDistortionUV = in.Input_BlendUVDistortionUV;
    Input.FlipbookIndex = in.Input_FlipbookIndex;
    Input.AlphaThreshold = in.Input_AlphaThreshold;
    VS_Output flattenTemp = _main(Input, v_257);
    out.gl_Position = flattenTemp.Position;
    out._entryPointOutput_Color = flattenTemp.Color;
    out._entryPointOutput_UV = flattenTemp.UV;
    out._entryPointOutput_Pos = flattenTemp.Pos;
    out._entryPointOutput_PosU = flattenTemp.PosU;
    out._entryPointOutput_PosR = flattenTemp.PosR;
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

