//#version 130

varying vec2 vPos;
varying vec4 vColor;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vPos = gl_Vertex.xy;
}
