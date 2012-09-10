uniform float ambient;
uniform vec4 ambientColor;
uniform float lightAmbient;
 
uniform sampler2D ColorMapSampler;
uniform sampler2D ShadingMapSampler;

void main()
{
    vec4 color2 = texture2D(ColorMapSampler, vec2(gl_TexCoord[0]));
    vec4 shading = texture2D(ShadingMapSampler, vec2(gl_TexCoord[0]));
 
    gl_FragColor = (color2 * ambientColor * ambient);
 
    gl_FragColor += color2 * shading * lightAmbient;
}