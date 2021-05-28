// TODO: add texture coordinates

attribute vec3 position;
attribute vec2 uv;

uniform mat4 modelMatrix;
varying vec2 outUv;

void main(void)
{
	outUv = uv;
	gl_Position = modelMatrix * vec4(position, 1.0);
}


