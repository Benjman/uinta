#include <uinta/debug.h>

#include <uinta/gl.h>
#include <uinta/shader.h>
#include <uinta/text/font.h>
#include <uinta/data/arr_utils.h>
#include <uinta/debug/debug_ui_controller.h>


namespace uinta {

	namespace debuguicontroller {

		static const char *vertexShader =
				"#version 300 es\n"
				"layout (location = 0) in lowp vec2 inPos;"
				"layout (location = 1) in lowp vec2 inTexCoord;"
				"out lowp vec2 passTexCoord;"
				"void main() {"
				"	gl_Position = vec4(inPos, 0.0, 1.0);"
				"	passTexCoord = vec2(inTexCoord.x, inTexCoord.y);"
				"}";

		static const char *fragShader =
				"#version 300 es\n"
				"in lowp vec2 passTexCoord;"
				"out lowp vec4 outColor;"
				"uniform sampler2D texture1;"
				"void main() {"
				"	outColor = vec4(texture(texture1, passTexCoord).r);"
				"}";

	}
	using namespace debuguicontroller;

	DebugUiController::DebugUiController() :
			_shader(Shader::createShader(vertexShader, fragShader, Raw)),
			_font(Font::loadFont("/usr/share/fonts/TTF/DejaVuSans-Bold.ttf")) {
	}

	void DebugUiController::initialize() {
		initializeControllers();
		initializeBuffers();
	}

	void DebugUiController::initializeControllers() { initFps(); }

	void DebugUiController::initFps() {
		_fpsLbl.initialize();
		_fps.initialize();
		shiftArr<uint32_t>(_fpsLbl.getCharCount() * 4, _fps.getIndexBuffer(), _fps.getCharCount() * 6);

		_vertexBufferSize += _fps.getVertexBufferSize() + _fpsLbl.getVertexBufferSize();
		_indexBufferSize += _fps.getIndexBufferSize() + _fpsLbl.getIndexBufferSize();
	}

	void DebugUiController::update(float_t dt) {
		_fps.update(dt);
	}

	void DebugUiController::render() {
		_fps.frameRendered();

		_shader->use();
		_font->bind();
		glDrawElements(GL_TRIANGLES, 6 * _fpsLbl.getCharCount(), GL_UNSIGNED_INT, (void *) (0 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, 6 * _fps.getCharCount(), GL_UNSIGNED_INT, (void *) (_fpsLbl.getIndexBufferSize()));
		glCheckError(GL_DRAW_ELEMENTS);
	}

	DebugUiController::~DebugUiController() {
		delete _font;
		delete _shader;
		delete _vao;
		delete _vertexBuffer;
		delete _indexBuffer;
	}

	void DebugUiController::initializeBuffers() {
		_vao = Vao::requestVao();

		_vertexBuffer = Vbo::requestVbo(_vao, GL_ARRAY_BUFFER, GL_STATIC_DRAW, _vertexBufferSize);
		_vertexBuffer->storeData(_fpsLbl.getVertexBuffer(), _fpsLbl.getVertexBufferSize());
		_vertexBuffer->storeData(_fps.getVertexBuffer(), _fps.getVertexBufferSize(), _fpsLbl.getVertexBufferSize());

		_indexBuffer = Vbo::requestVbo(_vao, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, _indexBufferSize);
		_indexBuffer->storeData(_fpsLbl.getIndexBuffer(), _fpsLbl.getIndexBufferSize());
		_indexBuffer->storeData(_fps.getIndexBuffer(), _fps.getIndexBufferSize(), _fpsLbl.getIndexBufferSize());

		_vao->createAttribute(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float_t), (void *) (0 * sizeof(float_t)));
		_vao->createAttribute(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float_t), (void *) (2 * sizeof(float_t)));
	}

	void DebugUiController::FpsTextController::update(float_t dt) {
		timeToNextUpdate -= dt;
		if (timeToNextUpdate <= 0) {
			_text->getValue() = std::to_string((size_t) std::ceil(frameCount * (1 / INTERVAL)));
			timeToNextUpdate += INTERVAL;
			initialize();
			DebugUiController *parent = ((DebugUiController*)getParent());
			parent->_vertexBuffer->storeData(getVertexBuffer(), getVertexBufferSize(), parent->_fpsLbl.getVertexBufferSize());
			frameCount = 0;
		}
	}
}
