#version 400
in VertexData{
  vec2 UV;
  vec3 color;
} inp;
out vec4 frag_color;
uniform sampler2D tex;
uniform vec2 UV_offset;
void main() {
  frag_color = texture( tex, inp.UV + UV_offset );
}