#shader vertex
#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 vertexColor;

out vec4 theColor;

uniform mat4 fullMatrix;

void main(){
    vec4 v = vec4(position, 1.0f);
    gl_Position = fullMatrix * v;
    theColor = vec4(vertexColor);
}

#shader fragment
#version 460 core

out vec4 finalColor;
in vec4 theColor;

void main(){
    finalColor = theColor;
}
