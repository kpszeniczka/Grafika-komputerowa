#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_Normals;
in vec4 v_FragPositionLight;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform sampler2D shadowMap;
uniform sampler2D textureSampler;

const vec4 LightColor = vec4(1.0f);
const float outerCone = 3.90f;
const float innerCone = 0.95f;
const float ambient = 0.20f;

vec4 SpotLight(vec4 textureColor) 
{
    vec3 normal = normalize(v_Normals);
    vec3 lightDirection = normalize(lightPos - v_Position);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    float specular = 0.0f;
    if (diffuse > 0.0f)
    {
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - v_Position);
        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
        specular = specAmount * specularLight;
    }

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    float shadow = 0.0f;
    vec3 lightCoords = v_FragPositionLight.xyz / v_FragPositionLight.w;
    if (lightCoords.z <= 1.0f)
    {
        lightCoords = (lightCoords + 1.0f) * 0.5f;
        float currentDepth = lightCoords.z;
        float bias = max(0.00025f * (1.0f - dot(normal, lightDirection)), 0.000005f);

        int sampleRadius = 4;
        vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
        for (int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for (int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0f;
            }
        }
        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    vec4 litColor = textureColor * (diffuse * (1.0f - shadow) * inten + ambient) + textureColor * specular * (1.0f - shadow) * inten;
    vec4 shadedColor = vec4(0.0f, 0.0f, 0.0f, textureColor.a);

    return mix(shadedColor, litColor, 1.0f - shadow);
}

void main()
{
    vec4 textureColor = texture(textureSampler, v_TexCoord);
    color = SpotLight(textureColor);
}
