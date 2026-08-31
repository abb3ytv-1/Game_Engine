#pragma once
#include"Mesh.h"

namespace nu {

	class Model {
		public:
			Model() = default;
			Model(const std::vector<Mesh>& meshes) : a_meshes{meshes}{}

			void AddMesh(const Mesh& mesh) { a_meshes.push_back(mesh); }

			void SetMeshes(const std::vector<Mesh>& meshes) { a_meshes = meshes; }
			const std::vector<Mesh>& GetMeshes() const { return a_meshes; }

		private:
			std::vector<Mesh> a_meshes;
	};
}