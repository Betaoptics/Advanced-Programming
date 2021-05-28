uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform vec4 materialEmissive;
uniform float specularPower;

varying vec2 outUv;
varying vec2 outUv2;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
    vec3 normal = normalize(eyespaceNormal);
    vec3 surfaceToLight = normalize(lightPosition - eyespacePosition);
    float d = dot(normal, surfaceToLight);
    vec4 diffuse = materialAmbient + materialDiffuse * d;
    diffuse.a = materialDiffuse.a;
    diffuse = texture2D(texture01, outUv) * diffuse + texture2D(texture02, outUv2);
    if (specularPower > 0.9)
    {
        vec3 surfaceToCamera = normalize(cameraPosition - eyespacePosition);
        float specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))),
            specularPower);
        vec4 specular = materialSpecular * specularCoefficient * texture2D(texture03, outUv).r;
        gl_FragColor = diffuse + materialEmissive + specular;
    }
    else
    {
        gl_FragColor = diffuse + materialEmissive;
    }
}

