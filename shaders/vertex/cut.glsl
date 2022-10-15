#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 2) in vec3 normal_modelspace;

uniform mat4 MVP;
uniform mat4 matGeo;
out vec3 normalV;
out vec3 position;

void main(){	
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	normalV=normal_modelspace;
	position=vertexPosition_modelspace;
}
