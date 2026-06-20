//cbuffer Constants : register(b0) { float4x4 g_ModelToClip; };
//Texture2D    t_Texture : register(t0);
//SamplerState s_Sampler : register(s0);
//
//void main_vs(float3 pos : POSITION, float2 uv : TEXCOORD0,
//             out float4 o_pos : SV_Position, out float2 o_uv : TEXCOORD0)
//{
//    o_pos = mul(float4(pos, 1.0), g_ModelToClip);
//    o_uv  = uv;
//}
//
//float4 main_ps(float2 uv : TEXCOORD0) : SV_Target
//{
//    return t_Texture.Sample(s_Sampler, uv);
//}



static const float2 g_positions[] = {
	float2(-0.5, -0.5),
	float2(0, 0.5),
	float2(0.5, -0.5)
};

static const float3 g_colors[] = {
	float3(1, 0, 0),
	float3(0, 1, 0),
	float3(0, 0, 1)
};

void main_vs(
	uint i_vertexId : SV_VertexID,
	out float4 o_pos : SV_Position,
	out float3 o_color : COLOR
)
{
	o_pos = float4(g_positions[i_vertexId], 0, 1);
	o_color = g_colors[i_vertexId];
}

void main_ps(
	in float4 i_pos : SV_Position,
	in float3 i_color : COLOR,
	out float4 o_color : SV_Target0
)
{
	o_color = float4(i_color, 1);
}