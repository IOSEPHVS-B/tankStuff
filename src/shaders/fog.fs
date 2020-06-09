#version 120

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// NOTE: Add here your custom variables
uniform vec4 fogColor;
uniform float fogDensity;
varying float zPos;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragTexCoord);
    
    // NOTE: Implement here your fragment shader code
    float fogAmount = 1 / pow(2.718, (zPos * fogDensity / 4))/10;
    fogAmount = clamp(fogAmount, 0.0f, 1.0f);
    
    //gl_FragColor = texelColor*colDiffuse;
    vec4 finalColor = texelColor*colDiffuse*fragColor;
    gl_FragColor = mix(finalColor, vec4(fogColor.rgb, finalColor.a), fogAmount);
}