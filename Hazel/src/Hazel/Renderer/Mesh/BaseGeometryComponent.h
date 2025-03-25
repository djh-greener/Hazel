#pragma once
#include"Hazel/Core/Core.h"
#include"Hazel/Scene/Entity.h"
#include<filesystem>
namespace Hazel {
	class StaticMesh;
	class Shader;
	class BaseGeometryComponent {
	public:
		//TODO: Sphere,			Cylinder
		enum GeometryType {
			None,Cube, Sphere
		};
		BaseGeometryComponent() {		Type = GeometryType::None;		}

		void SetTypeName(std::string TypeName);
		void SetType(GeometryType type);
		std::string GetTypeName() {
			std::vector<std::string>TypeNames = { "None","Cube","Sphere","Cylinder" };
			return TypeNames[(int)Type];
		}
		GeometryType& GetType() { return Type; }

		void SetTexturePath(std::filesystem::path path);
		std::filesystem::path GetTexturePath()const;
		void DrawMesh(Ref<Shader>&shader);
	private:
		void generateCube();
		void generateSphere();
		void generateCylinder();

		std::filesystem::path TexturePath;
		GeometryType Type;
	public:
		Entity Owner;
		Ref<StaticMesh> m_StaticMesh;

	};
}
