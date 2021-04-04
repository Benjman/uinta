#ifndef UINTA_TEXT_H
#define UINTA_TEXT_H

#include "../controller/controller.h"

#include <uinta/ui/ui.h>
#include <uinta/ui/ui_element.h>

#include <string>

namespace uinta {

	class Font;

	class Text : public UiElement {
		friend class TextController;

		std::string _value;

	public:
		explicit Text(const char *value, UiElement *parent = nullptr) :
				UiElement(parent, 0, 0, 0, 0, 0),
				_value(value) {}

		[[nodiscard]] std::string &getValue() {
			return _value;
		}

	}; // class Text

} // namespace uinta

#endif //UINTA_TEXT_H
