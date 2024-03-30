struct VertexStageInput
{
    float3 position : POSITION;
};

struct FramentStageInput
{
};

struct Fragment
{
    float4 color;
};

struct VertexStageOutput
{
    float4 sv_position     : SV_Position;
};

[shader("vertex")]
VertexStageOutput vertexMain(VertexStageInput vertexInput)
{
    VertexStageOutput output;

    float3 position = vertexInput.position;

    output.sv_position = float4(position, 1.0);

    return output;
}

[shader("fragment")]
float4 fragmentMain(FramentStageInput fragmentInput : FramentStageInput) : SV_Target
{
    return float4(0.0, 1.0, 0.0, 1.0);
}