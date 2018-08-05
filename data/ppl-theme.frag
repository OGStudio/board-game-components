
// Define floating point precision for GLES.
#ifdef GL_ES
    precision highp float;
#endif

// Per-pixel (per-fragment) lighting.
varying vec3 position;
varying vec3 normal;
// TODO Use uniform for light position.
const vec3 lightPosition = vec3(0, -10, 30);

varying vec2 faceTexCoord;
varying vec2 normalStateTexCoord;
varying vec2 selectedStateTexCoord;

// Image to texture with.
uniform sampler2D image;
// If we want selected texture coordinates or normal ones.
uniform bool isSelected;

float lambertianReflectanceStrength(vec3 position, vec3 lightPosition)
{
    // Light direction.
    vec3 lightVector = normalize(lightPosition - position);
    // Light flux strength.
    float strength = dot(normal, lightVector);
    float diffuse = max(strength, 0.2);
    // Add attenuation.
    //float distance = length(lightPosition - position);
    //return diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
    return diffuse;
}

void main()
{
    //vec2 pos = faceTexCoord;
    
    // Take normal or selected state color as the base one.
    vec3 color =
        isSelected ?
        texture2D(image, selectedStateTexCoord).rgb :
        texture2D(image, normalStateTexCoord).rgb;

    // Mix the base color with face one if face color is not transparent.
    vec4 faceColor = texture2D(image, faceTexCoord);
    if (faceColor.a > 0.0)
    {
        color = mix(color, vec3(faceColor), faceColor.a);
    }

    // TODO:  Use pos instead of position?
    // Provide per-pixel (per-fragment) lighting.
    color *= lambertianReflectanceStrength(position, lightPosition);
    
    gl_FragColor = vec4(color, 1.0);
}

