//#version 130

// z is some screen depth,
// in 2d camera is directed in one direction
// scene Z is 0, axis is faced to camera

uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
