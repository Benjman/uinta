#ifndef UINTA_UI_MESH_GENERATOR_H
#define UINTA_UI_MESH_GENERATOR_H

#include "ui.h"

#include <uinta/render/i_renderable.h>

#include <glm/vec2.hpp>
#include <vector>

namespace uinta {

	// TODO parent child relationship -- getRelativePosition() and getScreenPosition()
	// TODO Left, center, right anchoring

	class UiElement : public IRenderable {
		using Children = std::vector<UiElement*>;

		UiElement *_parent;
		glm::uvec2 _position{};
		glm::uvec2 _size{};
		float_t _scale = 1.f;
		Children _children;

		// ------------------------------
		// 	style stuff


	public:

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
			_parent(parent),
			_position(glm::uvec2(xPx, yPx)),
			_size(glm::uvec2(widthPx, heightPx)) {
			_offset = offset;
			if (_parent == nullptr && MAIN_UI_ELEMENT != nullptr) {
				_parent = MAIN_UI_ELEMENT;
			}
			if (_parent != nullptr) {
				_parent->addChild(this);
			}
		}

		void addChild(UiElement *element);

		void removeChild(UiElement *element);

		~UiElement() {
			if (_parent != nullptr) {
				_parent->removeChild(this);
			}
		}

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

		virtual void generateMesh(float_t *data, uint32_t *indices);

		[[nodiscard]] float_t getScale() const {
			return _scale;
		}

		void setScale(float_t scale) {
			_scale = scale;
		}

	}; // struct UiElement

} // namespace uinta

#endif //UINTA_UI_MESH_GENERATOR_H
