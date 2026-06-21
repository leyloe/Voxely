
#pragma pack_matrix(row_major)

cbuffer Constants : register(b0)
{
    float4x4 g_ModelToClip;
}

Texture2D    t_Texture : register(t0);
SamplerState s_Sampler : register(s0);

struct VsOut
{
    float4 pos : SV_Position;
    float2 uv  : TEXCOORD0;
};

VsOut main_vs(float3 pos : POSITION, float2 uv : TEXCOORD0)
{
    VsOut o;
    o.pos = mul(float4(pos, 1.0), g_ModelToClip);
    o.uv  = uv;
    return o;
}

float4 main_ps(VsOut input) : SV_Target
{
    return t_Texture.Sample(s_Sampler, input.uv);
}
