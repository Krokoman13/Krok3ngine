struct Interpolants {
    float4 position : SV_Position;
    
    float4 color : COLOR0;
    float3 normal : NORMAL;
    float3 fragPosition : FRAGPOSITION;
};

struct Pixel {
    float4 color : SV_Target;
};

cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix invWorldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

Pixel main(Interpolants In) {
    Pixel Out;
    
    float3 cameraPos = float4(viewMatrix._14, viewMatrix._24, viewMatrix._34, 1.f);
    
    float3 lightPos = float3(30.f, 30.f, 30.f);
    float3 pointLightColor = float3(1.f, 1.f, 1.f);    
    
    float3 ambientLightColor = float3(1.f, 1.f, 1.f);
    float ambientStrength = 0.5f;
    
    float3 ambient = ambientLightColor * ambientStrength;
    
    float3 diffrence = lightPos - In.fragPosition;
    float3 lightVector = normalize(diffrence);

    float diff = max(dot(In.normal, lightVector), 0.0);
    float3 diffuse = pointLightColor * diff;

    // Multiply the color by the diffuse illumination level to get final output color.
    Out.color = float4(ambient + diffuse, 1.0f) * In.color;
    return Out;
}
