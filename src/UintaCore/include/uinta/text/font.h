#ifndef UINTA_FONT_H
#define UINTA_FONT_H

#include <uinta/types.h>

namespace uinta {

	class Texture;

	class Font {
		friend class FontLoader;

	public:
		static Font loadFont(const char *trueTypePath);

	private:
		Texture &_texture;

		Font(Texture &texture) : _texture(texture) {}

	}; // class Font

} // namespace uinta

#endif //UINTA_FONT_H
