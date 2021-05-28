attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4		modelViewProjectionMatrix;
uniform mat4		modelMatrix;
uniform mat3		normalMatrix;
uniform float		uvOffset;

varying vec2 outUv;
varying vec2 outUv2;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
	vec4 vertexPosition = vec4(position, 1.0);
	outUv = uv;
	eyespacePosition = (modelMatrix * vertexPosition).xyz;
	eyespaceNormal = normalMatrix * normal;
	outUv2 = uv + uvOffset;
	gl_Position = modelViewProjectionMatrix * vertexPosition;
}

