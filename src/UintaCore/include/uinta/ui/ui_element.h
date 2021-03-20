#ifndef UINTA_UI_MESH_GENERATOR_H
#define UINTA_UI_MESH_GENERATOR_H

#include <uinta/render/i_renderable.h>

#include <glm/glm.hpp>
#include <vector>

namespace uinta {
	
	// TODO parent child relationship
	// TODO Left, center, right anchoring

	class UiElement : public IRenderable {
		using Children = std::vector<UiElement*>;

		UiElement *_parent;
		glm::uvec2 _position{};
		glm::uvec2 _size{};
		float_t _scale = 1.f;
		Children _children;

	public:

		size_t getVertexCount() override {
			return 4;
		}

		size_t getIndexCount() override {
			return 6;
		}

		void render() override;

		/**
		 * @param xPx size relative to UI_BASE_SIZE (1080)
		 * @param yPx size relative to UI_BASE_SIZE (1080)
		 * @param widthPx size relative to UI_BASE_SIZE (1080)
		 * @param heightPx size relative to UI_BASE_SIZE (1080)
		 */
		UiElement(UiElement *parent, size_t xPx, size_t yPx, size_t widthPx, size_t heightPx, size_t offset = 0) :
			IRenderable(offset),
			_parent(parent),
			_position(glm::uvec2(xPx, yPx)),
			_size(glm::uvec2(widthPx, heightPx)) {}

		/**
		 * @param x size relative to UI_BASE_SIZE (1080)
		 * @param y size relative to UI_BASE_SIZE (1080)
		 */
		void setPositionPx(uint16_t x, uint16_t y) {
			_position = glm::uvec2(x, y);
		}

		/**
		 * @param x size relative to UI_BASE_SIZE (1080)
		 * @param y size relative to UI_BASE_SIZE (1080)
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
