#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
out vec4 v_Color;
out vec2 v_Position;
uniform vec2 u_Positions[1024];
uniform float u_Radiuses[1024];
uniform vec4 u_Colors[1024];

void main()
{
    gl_Position = vec4(position * u_Radiuses[gl_InstanceID] + u_Positions[gl_InstanceID], 1.0, 1.0);
	v_Color = u_Colors[gl_InstanceID];
	v_Position = position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 v_Position;
in vec4 v_Color;

void main()
{
    //color = vec4(v_Color);
	color = vec4(v_Color.xyz, length(v_Position) > .5 ? 0.0 : v_Color.w);
}