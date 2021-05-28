uniform sampler2D texture01;
varying vec2 outUv;

void main(void)
{
    gl_FragColor = texture2D(texture01, outUv);
}

