#type vertex
#version 420 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_BiTangent;
layout(location = 5) in int	 a_EntityID;


uniform mat4 u_Model;
layout (std140,binding = 0) uniform Camera
{
    mat4 u_Projection;
    mat4 u_View;
};


out VS_OUT
{
	//vec3 Normal;
	//vec3 FragPos;
	vec2 TexCoord;
}vs_out;
out flat int v_EntityID;
void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	//vs_out.Normal = vec3(transpose(inverse(u_View * u_Model)) * vec4(a_Normal, 1));
	//vs_out.FragPos = vec3(u_View * u_Model * vec4(a_Position, 1.0));
	vs_out.TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;
};





#type fragment
#version 420 core
in VS_OUT{
	//vec3 Normal;
	//vec3 FragPos;
	vec2 TexCoord;
}fs_in;
in flat int v_EntityID;


struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
};
uniform Material material;


out vec4 FragColor;
out int entityID;
void main()
{
	vec3 result = vec3(texture(material.diffuse1, fs_in.TexCoord));
	FragColor = vec4(result, 1.0);
	entityID = v_EntityID;
}
