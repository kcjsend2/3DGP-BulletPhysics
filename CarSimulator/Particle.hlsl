#define PARTICLE_TYPE_EMITTER	0
#define PARTICLE_TYPE_FLARE		0x0ff
#include "Common.hlsli"

struct VS_PARTICLE_INPUT
{
    float3 position : POSITION;
    float3 color : COLOR;
    float3 velocity : VELOCITY;
    float3 acceleration : ACCELERATION;
    float2 size : SIZE;
    float2 age : AGELIFETIME; //(Age, Lifetime)
    uint type : PARTICLETYPE;
};

VS_PARTICLE_INPUT VSParticleStreamOutput(VS_PARTICLE_INPUT input)
{
    return (input);
}

float3 GetParticleColor(float fAge, float fLifetime)
{
    float3 cColor = float3(1.0f, 1.0f, 1.0f);

    if (fAge == 0.0f)
        cColor = float3(0.0f, 1.0f, 0.0f);
    else if (fLifetime == 0.0f) 
        cColor = float3(1.0f, 1.0f, 0.0f);
    else
    {
        float t = fAge / fLifetime;
        cColor = lerp(float3(1.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 1.0f), t * 1.0f);
    }

    return (cColor);
}

void GetBillboardCorners(float3 position, float2 size, out float4 pf4Positions[4])
{
    float3 f3Up = float3(0.0f, 1.0f, 0.0f);
    float3 f3Look = normalize(cameraPos - position);
    float3 f3Right = normalize(cross(f3Up, f3Look));

    pf4Positions[0] = float4(position + size.x * f3Right - size.y * f3Up, 1.0f);
    pf4Positions[1] = float4(position + size.x * f3Right + size.y * f3Up, 1.0f);
    pf4Positions[2] = float4(position - size.x * f3Right - size.y * f3Up, 1.0f);
    pf4Positions[3] = float4(position - size.x * f3Right + size.y * f3Up, 1.0f);
}

void GetPositions(float3 position, float2 f2Size, out float3 pf3Positions[8])
{
    float3 f3Right = float3(1.0f, 0.0f, 0.0f);
    float3 f3Up = float3(0.0f, 1.0f, 0.0f);
    float3 f3Look = float3(0.0f, 0.0f, 1.0f);

    float3 f3Extent = normalize(float3(1.0f, 1.0f, 1.0f));

    pf3Positions[0] = position + float3(-f2Size.x, 0.0f, -f2Size.y);
    pf3Positions[1] = position + float3(-f2Size.x, 0.0f, +f2Size.y);
    pf3Positions[2] = position + float3(+f2Size.x, 0.0f, -f2Size.y);
    pf3Positions[3] = position + float3(+f2Size.x, 0.0f, +f2Size.y);
    pf3Positions[4] = position + float3(-f2Size.x, 0.0f, 0.0f);
    pf3Positions[5] = position + float3(+f2Size.x, 0.0f, 0.0f);
    pf3Positions[6] = position + float3(0.0f, 0.0f, +f2Size.y);
    pf3Positions[7] = position + float3(0.0f, 0.0f, -f2Size.y);
}

[maxvertexcount(9)]
void GSParticleStreamOutput(point VS_PARTICLE_INPUT input[1], inout PointStream<VS_PARTICLE_INPUT> output)
{
    VS_PARTICLE_INPUT particle = input[0];

    particle.age.x += fTimeElapsed;
    if (particle.age.x <= particle.age.y)
    {
        if (particle.type == PARTICLE_TYPE_EMITTER)
        {
            particle.color = float3(1.0f, 0.0f, 0.0f);
//			particle.age.x = 0.0f;

            output.Append(particle);

            float3 pf3Positions[8];
            GetPositions(particle.position, float2(particle.size.x * 1.25f, particle.size.x * 1.25f), pf3Positions);

            particle.color = float3(0.0f, 0.0f, 1.0f);
            particle.age.x = 0.0f;

            for (int j = 0; j < 8; j++)
            {
                particle.type = (j >= 4) ? PARTICLE_TYPE_EMITTER : PARTICLE_TYPE_FLARE;
                particle.position = pf3Positions[j].xyz;
                particle.velocity = float3(0.0f, particle.size.x * particle.age.y * 4.0f, 0.0f) * 2.0f;
                particle.acceleration = float3(0.0f, 250.125f, 0.0f) * abs(f4Random.x);
                particle.age.y = (particle.type == PARTICLE_TYPE_EMITTER) ? 0.25f : 1.5f + (0.75f * abs(j - 4));

                output.Append(particle);
            }
        }
        else
        {
            particle.color = GetParticleColor(particle.age.x, particle.age.y);
            particle.position += (0.5f * particle.acceleration * fTimeElapsed * fTimeElapsed) + (particle.velocity * fTimeElapsed);

            output.Append(particle);
        }
    }
}

VS_PARTICLE_INPUT VSParticleDraw(VS_PARTICLE_INPUT input)
{
    return (input);
}

struct GS_PARTICLE_OUTPUT
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 uv : TEXCOORD;
    float2 age : AGELIFETIME; //(Age, Lifetime)
    uint type : PARTICLETYPE;
};

static float2 gf2QuadUVs[4] = { float2(0.0f, 1.0f), float2(0.0f, 0.0f), float2(1.0f, 1.0f), float2(1.0f, 0.0f) };

[maxvertexcount(4)]
void GSParticleDraw(point VS_PARTICLE_INPUT input[1], inout TriangleStream<GS_PARTICLE_OUTPUT> outputStream)
{
    float4 pVertices[4];
    GetBillboardCorners(mul(float4(input[0].position, 1.0f), gmtxWorld).xyz, input[0].size * 0.5f, pVertices);

    GS_PARTICLE_OUTPUT output = (GS_PARTICLE_OUTPUT) 0;
    output.color = input[0].color;
    output.age = input[0].age;
    output.type = input[0].type;
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(pVertices[i], gmtxViewProj);
        output.uv = gf2QuadUVs[i];

        outputStream.Append(output);
    }
}

float4 PSParticleDraw(GS_PARTICLE_OUTPUT input) : SV_TARGET
{
    float4 cColor = gtxtParticle.Sample(gsamLinearWrap, input.uv);
    if (input.type == PARTICLE_TYPE_FLARE)
    {
        cColor.rgb *= GetParticleColor(input.age.x, input.age.y);
    }

    return (cColor);
}
