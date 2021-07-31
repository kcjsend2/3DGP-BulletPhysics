struct MATERIAL //머티리얼 정보
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emmesive;
    float specularPower;
    float3 cColor;
};

//게임 객체의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
    MATERIAL material : packoffset(c4);
};

//카메라의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxViewProj : packoffset(c0);
    float3 cameraPos : packoffset(c4);
};

cbuffer cbLightInfo : register(b2)
{
    int nLights : packoffset(c0);
}

Texture2D gShadowMap : register(t0);

struct INSTANCED_GAMEOBJECT_INFO
{
    matrix m_mtxGameObject;
};
StructuredBuffer<INSTANCED_GAMEOBJECT_INFO> gGameObjectInfos : register(t0, space1);

struct LIGHT_INFO
{
    int type;
    float3 position;
    float4 ambient;
    float4 diffuse;
    float4 specualr;
    float3 attenuation;
    float range;
    float3 direction;
};
StructuredBuffer<LIGHT_INFO> light : register(t1, space1);


//정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_DEFAULT_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

//정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_DEFAULT_OUTPUT
{
    float4 position : SV_POSITION;
    float3 position_w : POSITION;
    float3 normal : NORMAL;
};

struct VS_INSTANCING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_INSTANCING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 position_w : POSITION;
    float3 normal : NORMAL;
};


float4 DirectionalLight(LIGHT_INFO directionalLight, float3 normal, float3 toCamera)
{
    float4 ambient = material.ambient * directionalLight.ambient;
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    float3 toLight = -directionalLight.direction;
    float diffuseFactor = dot(toLight, normal);
    if (0.0f < diffuseFactor)
    {
        float3 reflection = reflect(-toLight, normal);
        float specularFactor = pow(max(dot(reflection, toCamera), 0.0f), material.specularPower);
		
        diffuse = material.diffuse * (directionalLight.diffuse * diffuseFactor);
        specular = material.specular * (directionalLight.specualr * specularFactor);
    }
	
    return ambient + diffuse + specular + material.emmesive;
}

float4 PointLight(LIGHT_INFO pointLight, float3 position, float3 normal, float3 toCamera)
{
    float3 toLight = pointLight.position - position;
    float distance = length(toLight);
    if (pointLight.range < distance)
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    else
    {
        float4 ambient = material.ambient * pointLight.ambient;
        float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
        float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
        toLight /= distance;
        float diffuseFactor = dot(toLight, normal);
        
        if (0.0f < diffuseFactor)
        {
            float3 reflection = reflect(-toLight, normal);
            reflection = normalize(reflection);
            
            float4 specularFactor = pow(max(dot(reflection, toCamera), 0.0f), material.specularPower);
            
            diffuse = material.diffuse * (pointLight.diffuse * diffuseFactor);
            specular = material.specular * (pointLight.specualr * specularFactor);
        }
        
        float attenuation = 1.0f / dot(pointLight.attenuation, float3(1.0f, distance, distance * distance));
        
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        
        return ambient + diffuse + specular + material.emmesive;
    }
}

#define SPOT_LIGHT 0x00
#define DIRECTIONAL_LIGHT 0x01
#define POINT_LIGHT 0x02

float4 ComputeLight(LIGHT_INFO li, float3 pos, float3 nor, float3 toCamera)
{
    toCamera = normalize(toCamera);
    nor = normalize(nor);
    switch (li.type)
    {
        case POINT_LIGHT:
            return PointLight(li, pos, nor, toCamera);
            break;

        case DIRECTIONAL_LIGHT:
            return DirectionalLight(li, nor, toCamera);
            break;
    }
    
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}



VS_DEFAULT_OUTPUT VS_Default(VS_DEFAULT_INPUT input)
{
    VS_DEFAULT_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxViewProj);
    output.position_w = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gmtxWorld).xyz);
    
    return (output);
}

float4 PS_Default(VS_DEFAULT_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < nLights; i++)
    {
        cColor += ComputeLight(light[i], input.position_w, input.normal, normalize(cameraPos - input.position_w));
    }
    
    return (cColor);
}

VS_INSTANCING_OUTPUT VS_Instancing(VS_INSTANCING_INPUT input, uint InstanceID : SV_InstanceID)
{
    VS_INSTANCING_OUTPUT output;
    output.position = mul(mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject), gmtxViewProj);    
    output.position_w = mul(float4(input.position, 1.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz;
    output.normal = normalize(mul(float4(input.normal, 0.0f), gGameObjectInfos[InstanceID].m_mtxGameObject).xyz);
    
    return (output);
}

float4 PS_Instancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    for (int i = 0; i < nLights; i++)
    {
        cColor += ComputeLight(light[i], input.position_w, input.normal, normalize(cameraPos - input.position_w));
    }
    
    return (cColor);
}