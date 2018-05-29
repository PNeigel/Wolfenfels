#version 400
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 UV_in;
layout(location = 2) in vec3 color_in;
layout(location = 3) in float affected;

uniform mat4 MVP;
uniform mat4 animTransform;

out VertexData{
  vec2 UV;
  vec3 color;
} outp;

void main() {
  outp.UV = UV_in;
  outp.color = color_in;
  gl_Position = vec4(pos, 1.0);
  if (affected > 0.0) {
	gl_Position = animTransform * gl_Position;
  }
  gl_Position = MVP * gl_Position;
}