//--------------------------------------------------------------------------------------
// VertexShader.hlsl
//
// Simple vertex shader for rendering a triangle
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

struct VertexInput
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};

struct Interpolants
{
    float4 position : SV_Position;
    float4 color : COLOR0;
};

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

Interpolants main(VertexInput In)
{
    matrix mvp = mul(mul(projectionMatrix, viewMatrix), worldMatrix);
    float4 pos = mul(mvp, In.position);
    
    Interpolants Out;
    Out.position = pos;
    Out.color = In.color;
    
    return Out;
}