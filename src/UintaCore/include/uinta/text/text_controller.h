#ifndef UINTA_TEXT_CONTROLLER_H
#define UINTA_TEXT_CONTROLLER_H

#include <uinta/controller/buffer_controller.h>
#include <uinta/gl_types.h>
#include <uinta/render/i_renderable.h>

namespace uinta {
	// TODO move me to controllers/

	class BufferController;
	class Font;
	class Mesh;
	class Text;

	class TextController : public Controller, public IRenderable {
	protected:
		Text *_text;
		Font *_font;
		Mesh *_mesh;

		size_t _maxChars = 0;

	public:
		static const size_t VERTICES_PER_CHAR = 4;
		static const size_t INDICES_PER_CHAR = 6;
		static const size_t ELEMENTS_PER_VERTEX = 4; // vec2 position, vec2 uv // TODO add color

		explicit TextController(BufferController *parent, Text &text, Font *font, size_t maxChars = 0);

		~TextController();

		void initialize() override;

		void render() override;

		void setTextValue(const char *value, bool updateMetadata = true);

		void populateMesh() const;

		void uploadMesh(BufferController *controller) const;

		void doUpdateMetadata();

		[[nodiscard]] size_t getIBufferLen() const { return getMaxChars() * INDICES_PER_CHAR; }
		[[nodiscard]] size_t getIBufferSize() const { return getIBufferLen() * sizeof(GLuint); }
		[[nodiscard]] size_t getICount() const;
		[[nodiscard]] size_t getMaxChars() const { return _maxChars; }
		[[nodiscard]] size_t getMaxIdxCount() const { return getMaxChars() * VERTICES_PER_CHAR; }
		[[nodiscard]] Mesh *getMesh() const { return _mesh; }
		[[nodiscard]] size_t getVBufferLen() const { return getMaxChars() * VERTICES_PER_CHAR * ELEMENTS_PER_VERTEX; }
		[[nodiscard]] size_t getVBufferSize() const { return getVBufferLen() * sizeof(GLfloat); }

	}; // class TextController

}// namespace uinta

#endif //UINTA_TEXT_CONTROLLER_H
