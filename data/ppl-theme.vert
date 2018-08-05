
// Per-pixel (per-fragment) lighting.
// Topic: Lambertian reflectance
// Source: http://www.learnopengles.com/tag/lambertian-reflectance/

// Tile theme assumes we have three texture coordinates:
// * tile face
// * tile normal state
// * tile selected state

varying vec3 position;
varying vec3 normal;

varying vec2 faceTexCoord;
varying vec2 normalStateTexCoord;
varying vec2 selectedStateTexCoord;

void main()
{
    // Pass vertex.
    // Convert from Model/Object space to Screen one.
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Pass position and normal in Camera/Eye space.
    position = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal = vec3(gl_ModelViewMatrix * vec4(gl_Normal, 0.0));

    // Pass texture coordinates.
    faceTexCoord = gl_MultiTexCoord0.xy;
    normalStateTexCoord = gl_MultiTexCoord1.xy;
    selectedStateTexCoord = gl_MultiTexCoord2.xy;
}

