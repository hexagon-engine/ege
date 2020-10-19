//#version 330

varying vec2 vPos;
varying vec4 vColor;

void main()
{
  float dst = sqrt(vPos.x * vPos.x + vPos.y * vPos.y);
  float maxdst = sqrt(10000);
  float a = (1.0 - dst / maxdst);
  gl_FragColor += vec4(a * dst / maxdst, 0.0, a, a);
}
