#ifndef UINTA_UI_MESH_GENERATOR_H
#define UINTA_UI_MESH_GENERATOR_H

#include <glm/glm.hpp>

namespace uinta {

	class UiElement {
		glm::uvec2 _position{};
		glm::uvec2 _size{};

	public:
		static size_t getVertexCount() {
			return 4;
		}

		static size_t getIndexCount() {
			return 6;
		}

		/**
		 * @param xPx size relative to UI_BASE_WIDTH (1920)
		 * @param yPx size relative to UI_BASE_HEIGHT (1080)
		 * @param widthPx size relative to UI_BASE_WIDTH (1920)
		 * @param heightPx size relative to UI_BASE_HEIGHT (1080)
		 */
		UiElement(size_t xPx, size_t yPx, size_t widthPx, size_t heightPx) :
				_position(glm::uvec2(xPx, yPx)),
				_size(glm::uvec2(widthPx, heightPx)) {}

		/**
		 * @param x size relative to UI_BASE_WIDTH (1920)
		 * @param y size relative to UI_BASE_HEIGHT (1080)
		 */
		void setPositionPx(uint16_t x, uint16_t y) {
			_position = glm::uvec2(x, y);
		}

		/**
		 * @param x size relative to UI_BASE_WIDTH (1920)
		 * @param y size relative to UI_BASE_HEIGHT (1080)
		 */
		void setSizePx(uint16_t x, uint16_t y) {
			_size = glm::uvec2(x, y);
		}

		[[nodiscard]] const glm::uvec2 &getPosition() const {
			return _position;
		}

		[[nodiscard]] const glm::uvec2 &getSize() const {
			return _size;
		}

		void generateMesh(float_t *data, uint32_t *indices);
	}; // struct UiElement

} // namespace uinta

#endif //UINTA_UI_MESH_GENERATOR_H
