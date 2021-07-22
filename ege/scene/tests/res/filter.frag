varying vec4 vColor;

void main()
{
    // Filter out red
    gl_FragColor = vColor;
    gl_FragColor.r = 0;
}
