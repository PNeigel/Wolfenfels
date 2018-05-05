#version 400
in VertexData{
  vec2 UV;
  vec3 color;
} inp;
out vec4 frag_color;
uniform sampler2D tex;
void main() {
  frag_color = texture( tex, inp.UV );
}