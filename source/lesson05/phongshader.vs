attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4		modelViewProjectionMatrix;
uniform mat4		modelMatrix;
uniform mat3		normalMatrix;

varying vec2 outUv;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
	vec4 vertexPosition = vec4(position, 1.0);
	outUv = uv;
	eyespacePosition = (modelMatrix * vertexPosition).xyz;
	eyespaceNormal = normalMatrix * normal;
	gl_Position = modelViewProjectionMatrix * vertexPosition;
}

