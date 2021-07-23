uniform sampler2D ege_Image;
uniform vec2 ege_Size;

// https://www.geeks3d.com/20101029/shader-library-pixelation-post-processing-effect-glsl/
void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    
    vec3 tc = vec3(1.0, 0.0, 0.0);
    float dx = 2*(1./ege_Size.x);
    float dy = 2*(1./ege_Size.y);
    vec2 coord = vec2(dx*floor(uv.x/dx),
                    dy*floor(uv.y/dy));
    tc = texture2D(ege_Image, coord).rgb;
    gl_FragColor = vec4(tc, 1.0);
}
