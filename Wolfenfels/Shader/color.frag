#version 400
in VertexData{
  vec2 UV;
  vec3 color;
} inp;
out vec4 frag_color;
void main() {
  frag_color = vec4(inp.color, 1.0);
}