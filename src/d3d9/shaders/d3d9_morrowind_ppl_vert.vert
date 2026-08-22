#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_spirv_intrinsics : require

layout(location = 0) in vec4 in_Position0;
layout(location = 1) in vec4 in_Normal0;
layout(location = 4) in vec4 in_Texcoord0;
layout(location = 5) in vec4 in_Texcoord1;
layout(location = 6) in vec4 in_Texcoord2;
layout(location = 7) in vec4 in_Texcoord3;
layout(location = 12) in vec4 in_Color0;
layout(location = 16) in vec4 in_BlendWeight;

invariant gl_Position;

layout(location = 0) out vec4 out_NormalFog;
layout(location = 1) out vec4 out_Texcoord01;
layout(location = 2) out vec4 out_Texcoord23;
layout(location = 3) out vec4 out_ViewPosition;
layout(location = 4) out vec4 out_Color0;

#include "d3d9_morrowind_ppl_common.glsl"

vec2 loadPplTexcoord(uint index) {
    vec2 result = in_Texcoord0.xy;
    result = mix(result, in_Texcoord1.xy, bvec2(index == 1u));
    result = mix(result, in_Texcoord2.xy, bvec2(index == 2u));
    result = mix(result, in_Texcoord3.xy, bvec2(index == 3u));
    return result;
}

void transformPplVertex(out vec4 viewPosition, out vec3 viewNormal) {
    if (!hasPplFlag(MorrowindPplUseSkinning)) {
        viewPosition = in_Position0 * ppl.worldView[0];
        viewNormal = (vec4(in_Normal0.xyz, 0.0) * ppl.worldView[0]).xyz;
        return;
    }

    vec4 weights = in_BlendWeight;
    if (ppl.vertexBlendState == 1u)
        weights.y = 1.0 - weights.x;
    else if (ppl.vertexBlendState == 2u)
        weights.z = 1.0 - weights.x - weights.y;
    else if (ppl.vertexBlendState == 3u)
        weights.w = 1.0 - weights.x - weights.y - weights.z;

    viewPosition = (in_Position0 * ppl.worldView[0]) * weights.x;
    viewNormal = ((vec4(in_Normal0.xyz, 0.0) * ppl.worldView[0]).xyz) * weights.x;

    for (uint i = 1u; i <= ppl.vertexBlendState; i++) {
        viewPosition += (in_Position0 * ppl.worldView[i]) * weights[i];
        viewNormal += ((vec4(in_Normal0.xyz, 0.0) * ppl.worldView[i]).xyz) * weights[i];
    }
}

void main() {
    vec4 viewPosition;
    vec3 viewNormal;
    transformPplVertex(viewPosition, viewNormal);

    gl_Position = viewPosition * ppl.projection;

    float distanceToEye = length(viewPosition);
    float fog = clamp(
        (ppl.nearFogRange - distanceToEye)
            / (ppl.nearFogRange - ppl.nearFogStart),
        0.0,
        1.0);
    out_NormalFog = vec4(viewNormal, fog);

    vec2 routed[4];
    for (uint i = 0u; i < 4u; i++)
        routed[i] = loadPplTexcoord(i);

    if (hasPplFlag(MorrowindPplUseTexgen)) {
        MorrowindPplStage stage = ppl.stages[ppl.texgenStage];
        vec3 generated = vec3(0.0);

        switch (stage.texcoordGen) {
            case 1u:
                generated = normalize(viewNormal);
                break;
            case 2u:
                generated = viewPosition.xyz;
                break;
            case 3u:
                generated = reflect(normalize(viewPosition.xyz), normalize(viewNormal));
                break;
            case 4u:
                generated = vec3(0.5 * routed[stage.texcoordIndex] + 0.5, 0.0);
                break;
        }

        generated = (vec4(generated, 1.0) * ppl.texgenTransform).xyz;
        routed[ppl.uvSetCount] = generated.xy;
        if (hasPplFlag(MorrowindPplProjectiveTexgen))
            routed[ppl.uvSetCount + 1u] = generated.zz;
    }

    out_Texcoord01 = vec4(routed[0], routed[1]);
    out_Texcoord23 = vec4(routed[2], routed[3]);
    out_Color0 = hasPplFlag(MorrowindPplVertexColor) ? in_Color0 : vec4(1.0);

    // The fragment shader reconstructs per-light vectors from this, so varying
    // usage does not grow with the light limit.
    out_ViewPosition = viewPosition;
}
