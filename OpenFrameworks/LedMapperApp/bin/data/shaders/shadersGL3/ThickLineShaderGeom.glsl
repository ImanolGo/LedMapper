#version 150

uniform float thickness;

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

void main() {

  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  
  vec3 up = vec3(0, 0, 1);  // arbitrary up vector
  
  vec3 dir = normalize(p1 - p0);      // normalized direction vector from p0 to p1
  vec3 right = normalize(cross(dir, up)); // right vector
  vec3 norm = cross(right, dir);
  
  right *= thickness;


gl_Position = gl_in[0].gl_Position - vec4(right.x, right.y, right.z, 0.0);
EmitVertex();

gl_Position = gl_in[0].gl_Position + vec4(right.x, right.y, right.z, 0.0);
EmitVertex();

gl_Position = gl_in[1].gl_Position - vec4(right.x, right.y, right.z, 0.0);
EmitVertex();

gl_Position = gl_in[1].gl_Position + vec4(right.x, right.y, right.z, 0.0);
EmitVertex();

EndPrimitive();}