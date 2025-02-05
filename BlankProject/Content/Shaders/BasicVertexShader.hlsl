struct VertexInput {
    float3 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct Interpolants {
    float4 position : SV_Position;
    float4 color : COLOR0;
};

cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

Interpolants main(VertexInput In) {
    matrix mvp = mul(mul(projectionMatrix, viewMatrix), worldMatrix);
    float4 pos = mul(mvp, float4(In.position, 1.0f));
    
    Interpolants Out;
    Out.position = pos;
    Out.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return Out;
}