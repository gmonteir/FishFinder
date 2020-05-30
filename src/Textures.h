
#pragma once

#ifndef TEXTURES_INCLUDED
#define TEXTURES_INCLUDED

#include "Texture.h"

#include <unordered_map>
#include <vector>
#include <memory>

class Textures {

	Textures();

public:
	static std::shared_ptr<Textures> getInstance();

	~Textures() {}

	std::unordered_map<std::string, std::shared_ptr<Texture>> allTextures;
	void addTexture(const std::string& filename, const std::string& key, int mode);
	std::shared_ptr<Texture> getTexture(const std::string& key);

private:
	int currentUnit = 0;

	void initCausticsText();
};

#endif