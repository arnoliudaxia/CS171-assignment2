#version 330 core

in vec3 normalV;

out vec3 color;


void main(){
color = abs(normalV);
} 