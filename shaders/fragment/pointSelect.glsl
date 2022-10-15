#version 330 core
out vec4 color;
in vec3 normalV;

uniform vec3 selection;

void main(){
	if(normalV==selection)
	{
	color = vec4(.968f,.745f,0.094f,1.f);

	}
	else
	{
	color = vec4(.5f,.5f,.5f,0.3f);
	}
}