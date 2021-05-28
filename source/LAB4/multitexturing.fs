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
    float diffuseFactor = dot(normal, surfaceToLight);
    vec4 diffuse = texture2D(texture01, outUv) * materialDiffuse * diffuseFactor + texture2D(texture03, outUv2);
    diffuse.a = materialDiffuse.a;

    if (specularPower > 0.9)
    {
        vec3 surfaceToCamera = normalize(cameraPosition - eyespacePosition);
        float specularFactor = dot(surfaceToCamera, reflect(-surfaceToLight, normal));
        specularFactor = pow(max(0.0, specularFactor), specularPower);
        vec4 specular = materialSpecular * specularFactor * texture2D(texture02, outUv).r;
        gl_FragColor = materialAmbient + diffuse + materialEmissive + specular;
    }
    else
    {
        gl_FragColor = materialAmbient + diffuse + materialEmissive;
    }
}

