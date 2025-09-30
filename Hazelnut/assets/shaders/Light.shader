#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_BiTangent;
layout(location = 5) in int	 a_EntityID;

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 u_View;
	mat4 u_Projection;
	vec3  u_ViewPos;
};
uniform mat4 u_Model;

out flat int v_EntityID;
void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

	v_EntityID = a_EntityID;

}

#type fragment
#version 450 core

in flat int v_EntityID;

uniform vec3 u_Color;

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;
void main()
{
	color = vec4(u_Color, 1);
	color2 = v_EntityID;
}
