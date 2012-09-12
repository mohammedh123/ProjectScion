uniform float ambient;
uniform vec4 ambientColor;
uniform float lightAmbient;

uniform sampler2D colorMapSampler;
uniform sampler2D shadingMapSampler;

void main()
{
    vec4 color2 = texture2D(colorMapSampler, gl_TexCoord[0].xy);
    vec4 shading = texture2D(shadingMapSampler, gl_TexCoord[0].xy);
 
    vec4 finalColor = (color2 * ambientColor * ambient);
 
    finalColor += color2 * shading * lightAmbient;
 
    gl_FragColor = finalColor;
}