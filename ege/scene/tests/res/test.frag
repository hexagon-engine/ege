//#version 330

varying vec2 vPos;
varying vec4 vColor;

uniform float disturb1;
uniform float disturb2;

void main()
{
  vPos.x += disturb1 * sin(vPos.y / 10.0) * disturb2 * 5.0;
  vPos.y += (disturb2 + 0.2) * sin(vPos.x / 15.0) * (disturb1 - 0.2) * 5.0;
  float dst = sqrt(vPos.x * vPos.x + vPos.y * vPos.y);
  float maxdst = 100;
  float a = (1.0 - dst / maxdst);
  gl_FragColor += vec4(a * dst / maxdst, 0.0, a, a);
}
