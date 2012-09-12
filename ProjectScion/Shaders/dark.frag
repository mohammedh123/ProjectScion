uniform sampler2D texture;

uniform float screenWidth;
uniform float screenHeight;
uniform float lightStrength;
uniform float lightRadius;
uniform vec2 lightPosition;
uniform vec4 lightColor;

void main()
{
	vec4 color = 0.5*texture2D(texture, gl_TexCoord[0].xy);

	vec2 pixelPosition;
	pixelPosition.x = screenWidth * gl_TexCoord[0].x;
	pixelPosition.y = screenHeight * gl_TexCoord[0].y;

	vec2 lightDirection = lightPosition - pixelPosition;
	float distance = 1 / length(lightPosition-pixelPosition) * lightStrength;
	float amount = 20;

	float coneAttenuation = clamp(1.0f - length(lightDirection) / lightRadius, 0.0f, 1.0f);

	vec3 shading = distance*amount*coneAttenuation*lightColor.xyz;

	gl_FragColor = vec4(color.xyz + shading, 1.0f);
}