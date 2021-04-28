#ifndef UINTA_MODEL_H
#define UINTA_MODEL_H

#include <uinta/render/i_renderable.h>
#include <uinta/io/file.h>

namespace uinta {

	class BufferController;
	class Mesh;

	class Model : public IRenderable {
		BufferController *_buffer;
		std::string _path;
		Mesh **_children{};
		size_t _numChildren{};

		void loadModel();

	public:
		explicit Model(BufferController *buffer, const char *path) : _buffer(buffer), _path(File::getFilePath(path)) {}

		virtual void initialize() {
			loadModel();
		}

		~Model();

		void render() override;

	};

} // namespace uinta

#endif //UINTA_MODEL_H
