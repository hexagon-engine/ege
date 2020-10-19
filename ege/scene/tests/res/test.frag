//#version 330

varying vec2 vPos;
varying vec4 vColor;

void main()
{
  float dst = sqrt(vPos.x * vPos.x + vPos.y * vPos.y);
  gl_FragColor = vec4(dst / 144.0, 0, 1, 1);
}
