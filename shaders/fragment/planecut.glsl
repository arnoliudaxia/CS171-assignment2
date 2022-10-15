#version 330 core

in vec3 normalV;
in vec3 position;

out vec4 color;

uniform float cutzplane;

void main(){
if(position.z<cutzplane)
{
color = vec4(abs(normalV),1);
}
else
{
color = vec4(abs(normalV),0);
}
} 