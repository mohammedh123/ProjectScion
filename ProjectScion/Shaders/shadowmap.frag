uniform float lightStrength;
uniform float lightRadius;
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform float screenWidth;
uniform float screenHeight; 
 
void main()
{
	int depth = 1;

    vec3 pixelPosition;
    pixelPosition.x = screenWidth * gl_TexCoord[0].x;
    pixelPosition.y = screenHeight * gl_TexCoord[0].y;
    pixelPosition.z = 0.0f;
    //pixelPosition.w = 1.0f;
 
    vec3 shading;
    if (depth > 0)
    {
        vec3 lightDirection = lightPosition - pixelPosition;
        float distance = 1.0f / length(lightPosition - pixelPosition) * lightStrength;
        float amount = max(dot(depth, normalize(distance)), 0);
 
        float coneAttenuation = clamp(1.0f - length(lightDirection) / lightRadius, 0.0, 1.0);
 
        shading = distance * amount * coneAttenuation * lightColor;
    }
 
    gl_FragColor = vec4(shading.r, shading.g, shading.b, 1.0f);
}