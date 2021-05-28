attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightDirection;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float specularPower;

varying vec2 outUv;
varying vec4 outColor;

void main(void)
{
	mat4 mv = viewMatrix * modelMatrix;
	vec3 modelViewNormal = vec3(mv * vec4(normal, 0.0));
	float diffuseFactor = dot(modelViewNormal, -lightDirection);
	outUv = uv;

	vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
	if (specularPower > 0.5)
	{
		// calculate specular component
		vec3 eyespacePosition = normalize((modelMatrix * vec4(-position, 1.0)).xyz);
		vec3 ref = reflect(-lightDirection, modelViewNormal);
		float specularFactor = max(0.0, dot(ref, eyespacePosition));
		specular = materialSpecular * pow(specularFactor, specularPower);
		specular.a = 0.0;
	}
	outColor = materialDiffuse * diffuseFactor + specular;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}

