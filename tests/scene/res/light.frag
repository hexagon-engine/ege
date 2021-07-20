//#version 330

uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
  // A = y1 - y2
  // B = x2 - x1
  // C = x1*y2 - x2*y1;
  // calculate ray equation ([1] camPos -> [2] gl_FragCoord)
  float c2sA = camPos.y - gl_FragCoord.y;
  float c2sB = - camPos.y;
  float c2sC = camPos.z * gl_FragCoord.y;
  
  gl_FragColor += vec4(a * dst / maxdst, 0.0, a, a);
}
