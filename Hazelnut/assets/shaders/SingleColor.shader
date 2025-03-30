#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_BiTangent;
layout(location = 5) in int	 a_EntityID;

uniform mat4 u_Model;
layout(std140, binding = 0) uniform CameraBlock
{
	mat4 u_Projection;
	mat4 u_View;
	vec3  u_ViewPos;
};

void main()
{

	vec4 clipPos = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	vec3 worldNormal = normalize(mat3(u_Model) * a_Normal);
	vec4 clipNormal = u_Projection * u_View * vec4(worldNormal, 0.0);

	vec2 screenNormal = normalize(clipNormal.xy);
	clipPos.xy += screenNormal * 0.01 * clipPos.w;

	gl_Position = clipPos;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.44, 0.48, 0.06, 1.0);
}


