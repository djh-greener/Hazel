#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_BiTangent;
layout(location = 5) in int	 a_EntityID;

layout (std140,binding = 0) uniform CameraBlock
{
    mat4 u_Projection;
    mat4 u_View;
	vec3  u_ViewPos;
};
uniform mat4 u_Model;
out VertexOutput
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}Output;

out flat int v_EntityID;
void main()
{
	gl_Position = u_Projection * u_View  * u_Model * vec4(a_Position, 1.0);

	Output.FragPos = vec3(u_Model * vec4(a_Position, 1.0));
	Output.Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
	Output.TexCoord = a_TexCoord;

	v_EntityID = a_EntityID;

}

#type fragment
#version 450 core
in VertexOutput
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
}Input;
in flat int v_EntityID;


struct PointLight {
	vec3 Position;
	vec3 Color;
	float Linear;
	float Quadratic;
};
const int MAX_POINTLIGHTS = 32;
uniform int numPointLights;
layout (std140,binding = 1) uniform PointLightBlock
{
	PointLight PointLights[MAX_POINTLIGHTS];
};
//Redefine to use u_ViewPos
layout(std140, binding = 0) uniform CameraBlock
{
	mat4 u_Projection;
	mat4 u_View;
	vec3  u_ViewPos;
};

struct Material {
	sampler2D diffuse1;
	sampler2D specular1;
};
uniform Material material;


layout(location = 0) out vec4 color;
layout(location = 1) out int color2;
void main()
{
	vec3 texColor = texture(material.diffuse1, Input.TexCoord).rgb;

	vec3 lighting = texColor * 0.1; // ambient
	vec3 ViewDir = normalize(u_ViewPos - Input.FragPos);
	vec3 Normal = normalize(Input.Normal);
	for (int i = 0; i < numPointLights; ++i)
	{
		vec3 LightDir = normalize(PointLights[i].Position - Input.FragPos);
		vec3 HalfDir = normalize(LightDir + ViewDir);

		vec3 diffuse = max(dot(Normal, LightDir), 0.0) * texColor * PointLights[i].Color;
		vec3 specular = pow(max(dot(Normal, HalfDir), 0.0), 16.0) * texColor * PointLights[i].Color;

		float distance = length(PointLights[i].Position - Input.FragPos);
		float attenuation = 1.0 / (1.0 + PointLights[i].Linear * distance + PointLights[i].Quadratic * distance * distance);
		lighting += (diffuse + specular) * attenuation;
	}
	
	lighting = lighting / (lighting + vec3(1.0));
	lighting = pow(lighting, vec3(1.0 / 2.2));

	color = vec4(lighting,1);
	color2 = v_EntityID;
}
