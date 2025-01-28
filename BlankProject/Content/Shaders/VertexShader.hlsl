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
    float4 pos = mul(mul(mul(In.position, worldMatrix), viewMatrix), projectionMatrix);
    
    Interpolants Out;
    Out.position = pos;
    Out.color = In.color;
    
    return Out;
}