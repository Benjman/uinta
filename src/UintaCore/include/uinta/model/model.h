#ifndef UINTA_MODEL_H
#define UINTA_MODEL_H

#include <uinta/render/i_renderable.h>

namespace uinta {

	class BufferController;
	class Mesh;

	class Model : public IRenderable {
		Mesh **_children{};
		const size_t _numChildren;

		Model(Mesh **children, size_t numChildren);

	public:
		static Model loadModel(const char *path, BufferController *buffer);

		~Model();

		void render() override;

	};

} // namespace uinta

#endif //UINTA_MODEL_H
