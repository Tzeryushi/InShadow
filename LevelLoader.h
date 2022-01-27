#pragma once

#include <YabeEngine/ResourceAgent.h>
#include <YabeEngine/Errors.h>

#include "PlayerAgent.h"
#include "LightAgent.h"
#include "Triangle.h"
#include "Box.h"
#include "Quad.h"

#include <string>
#include <vector>
#include <fstream>

class LevelLoader
{
public:
	static void save(std::string& fp, PlayerAgent& player, LightAgent& light, std::vector<Triangle>& triangles, std::vector<Box>& boxes, glm::vec2 exitPos, float scale);

	static void load(std::string& fp, b2World* world, YabeEngine::Camera& camera, PlayerAgent& player, LightAgent& light, std::vector<Triangle>& triangles, std::vector<Box>& boxes, glm::vec2& exitPos);
};

