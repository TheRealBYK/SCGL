#shader vertex
#version 460 core

layout(location=0) in vec3 position;
layout(location=1) in vec4 vertexColor;

out vec4 theColor;

void main(){
    gl_Position = vec4(position, 1.0f);
    theColor = vec4(vertexColor);
}

#shader fragment
#version 460 core

out vec4 finalColor;
in vec4 theColor;

void main(){
    finalColor = theColor;
}
