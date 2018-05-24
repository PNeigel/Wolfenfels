#version 400
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 UV_in;
layout(location = 2) in vec3 color_in;

uniform float scale;
uniform float aspectRatio;
uniform vec3 cam_pos;

out VertexData{
  vec2 UV;
  vec3 color;

} outp;
void main() {
  outp.UV = UV_in;
  outp.color = color_in;
  vec3 centered = pos - cam_pos;
  vec3 aspect = vec3(1/aspectRatio, 1.0, 1.0);
  gl_Position = vec4(scale * aspect * centered, 1.0);
}