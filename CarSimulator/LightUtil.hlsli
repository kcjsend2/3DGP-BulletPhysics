struct MATERIAL //머티리얼 정보
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emmesive;
    float specularPower;
    float3 cColor;
};

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

cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
    MATERIAL material : packoffset(c4);
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

#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT 3

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