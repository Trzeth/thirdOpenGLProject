#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 viewPos;
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;

    // Shadow
    vec4 fragPosLightSpace;

    // Lighting
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} fs_in;

uniform sampler2D texture_diffuse;
uniform sampler2D shadowMap;
uniform int useColor;
uniform vec3 diffuseColor;

vec3 CalcDirLight(vec3 dir,vec3 a,vec3 d,vec3 s, vec3 normal, vec3 viewDir,vec3 color);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
    // 属性
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(fs_in.viewPos - fs_in.fragPos);
    vec3 color;
    if(useColor == 1)
    {
        color = diffuseColor;
    }
    else
    {
        color = vec3(texture(texture_diffuse, fs_in.texCoords).xyz);
    }

    vec3 result = CalcDirLight(fs_in.direction,fs_in.ambient,fs_in.diffuse,fs_in.specular, norm, viewDir,color);
    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(vec3 dir,vec3 a,vec3 d,vec3 s, vec3 normal, vec3 viewDir,vec3 color)
{
    vec3 lightDir = normalize(-dir);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(dir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);

    vec3 ambient  = a;
    vec3 diffuse  = d * diff;
    vec3 specular = s * spec;
    
    float shadow = ShadowCalculation(fs_in.fragPosLightSpace);

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * color;;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.normal);
    vec3 lightDir = fs_in.direction;
    float bias = max(0.0003 * (1.0 - dot(normal, lightDir)), 0.00003);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}