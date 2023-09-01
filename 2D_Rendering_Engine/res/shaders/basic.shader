#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
out vec3 v_Color;
uniform vec2 u_Positions[1024];
uniform vec3 u_Colors[1024];

void main()
{
    gl_Position = vec4(position + u_Positions[gl_InstanceID], 1.0, 1.0);
	v_Color = u_Colors[gl_InstanceID];
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 v_Color;

void main()
{
    color = vec4(v_Color, 1.0);
}