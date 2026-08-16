#version 460 core

in layout(location=0) vec3 position;
in layout(location=1) vec4 vertexColor;

out vec4 theColor;

void main(){
    gl_Position = vec4(position, 1.0f);
    theColor = vec4(vertexColor);
}
