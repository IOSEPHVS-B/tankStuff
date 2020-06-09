#version 120

// Input vertex attributes
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;


// NOTE: Add here your custom variables
varying float zPos;


void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord.xy;
    fragColor = vertexColor;

    vec4 worldPosition = mvp*vec4(vertexPosition, 1.0);
    zPos = -worldPosition.z;
    
    // Calculate final vertex position
    gl_Position = worldPosition;
    gl_Position.xy=floor(worldPosition.xy*32)/32;
    //gl_Position /= gl_Position.w;
}