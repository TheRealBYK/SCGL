// Copyright (c) 2026 BYK. All Rights Reserved.
//
// This file is part of SCGL.
//
// SCGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, under version 2 of the License.
//
// SCGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with SCGL. If not, see <https://www.gnu.org/licenses/>.
//
// Maintainer: BYK <bykdev@proton.me>

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
