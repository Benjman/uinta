#include "font_loader.h"

#include <uinta/font/font.h>

uinta::Font uinta::Font::loadFont(const char *trueTypePath) {
	return FontLoader::loadFond(trueTypePath);
}
