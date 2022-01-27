#include "LevelLoader.h"
#include <iostream>

/***********************************************************
LevelLoader::save saves the parameter objects to the designated
filepath (fp). In the event of an error the function will 
perform a non-fatal return. This function does not handle
pointers.
***********************************************************/
void LevelLoader::save(std::string & fp, PlayerAgent & player, LightAgent & light, std::vector<Triangle>& triangles, std::vector<Box>& boxes, glm::vec2 exitPos, float scale)
{
	//this format relies heavily on escapes and spaces to delimit variables
	//files must be clean

	//open up 
	std::ofstream fsave(fp);
	if (fsave.fail())
	{
		YabeEngine::softError(fp.c_str());
		return;
	}

	/*CURRENT LOAD ORDER*/
	//Player, Light, Triangles, Boxes, exit, scale
	//triangles and boxes have loop counters
	fsave << "ISF_FILE_START" << '\n';

	//player
	fsave << player.getPos().x << ' ' << player.getPos().y << ' ' << player.getDrawRadius() << ' ' << player.getHitRadius() << ' '
		<< player.getColor().r << ' ' << player.getColor().g << ' ' << player.getColor().b << ' ' << player.getColor().a << '\n';
	
	//light
	fsave << light.getPos().x << ' ' << light.getPos().y << ' ' << light.getRadius() << ' ' << light.getIntensity() << ' '
		<< light.getColor().r << ' ' << light.getColor().g << ' ' << light.getColor().b << ' ' << light.getColor().a << '\n';

	//triangles
	fsave << triangles.size() << '\n';

	for (int i = 0; i < triangles.size(); i++)
	{
		fsave << triangles[i].getPos().x << ' ' << triangles[i].getPos().y << ' ' << triangles[i].getVertices()[0].x << ' ' << triangles[i].getVertices()[0].y << ' '
			<< triangles[i].getVertices()[1].x << ' ' << triangles[i].getVertices()[1].y << ' ' << triangles[i].getVertices()[2].x << ' ' << triangles[i].getVertices()[2].y << ' '
			<< triangles[i].getColor().r << ' ' << triangles[i].getColor().g << ' ' << triangles[i].getColor().b << ' ' << triangles[i].getColor().a << ' '
			<< triangles[i].isFixed() << ' ' << triangles[i].isDynamic() << ' ' << triangles[i].getDensity() << ' ' << triangles[i].getFriction() << ' ' << triangles[i].getAngle() << '\n';
	}

	//boxes
	fsave << boxes.size() << '\n';

	for (int i = 0; i < boxes.size(); i++)
	{
		fsave << boxes[i].getPos().x << ' ' << boxes[i].getPos().y << ' ' << boxes[i].getDims().x << ' ' << boxes[i].getDims().y << ' '
			<< boxes[i].getColor().r << ' ' << boxes[i].getColor().g << ' ' << boxes[i].getColor().b << ' ' << boxes[i].getColor().a << ' '
			<< boxes[i].isFixed() << ' ' << boxes[i].isDynamic() << ' ' << boxes[i].getTexture().filePath << ' ' << boxes[i].getDensity() << ' ' << boxes[i].getFriction() << ' '
			<< boxes[i].getAngle() << '\n';
	}

	//exit
	fsave << exitPos.x << ' ' << exitPos.y << '\n';

	//scale
	fsave << scale << '\n';

}

/***********************************************************
LevelLoader::loads from a formatted file into the given
parameter objects. In the event of an error, the function will
perform a non-fatal return.
***********************************************************/
void LevelLoader::load(std::string& fp, b2World* world, YabeEngine::Camera& camera, PlayerAgent& player, LightAgent& light, std::vector<Triangle>& triangles, std::vector<Box>& boxes, glm::vec2& levelGate)
{
	std::ifstream fload(fp);
	if (fload.fail()) {
		YabeEngine::softError(fp.c_str());
	}

	std::string set;
	fload >> set;

	/*CURRENT LOAD ORDER*/
	//Player, Light, Triangles, Boxes, exit, scale
	//triangles and boxes have loop counters

	//player
	glm::vec2 pos;
	float drawRadius;
	float hitRadius;
	YabeEngine::Color color;
	fload >> pos.x >> pos.y >> drawRadius >> hitRadius >> color.r >> color.g >> color.b >> color.a;
	player.init(world, pos, drawRadius, hitRadius, color);
	
	//light
	float radius;
	float intensity;
	fload >> pos.x >> pos.y >> radius >> intensity >> color.r >> color.g >> color.b >> color.a;
	light.init(world, pos, radius, color, intensity);

	//triangles
	glm::vec2 vert1, vert2, vert3;
	bool fixed;
	bool dynamic;
	float density;
	float friction;
	float angle;
	unsigned int count;
	fload >> count;
	for (unsigned int i = 0; i < count; i++)
	{
		fload >> pos.x >> pos.y >> vert1.x >> vert1.y >> vert2.x >> vert2.y >> vert3.x >> vert3.y
			>> color.r >> color.g >> color.b >> color.a >> fixed >> dynamic >> density >> friction >> angle;

		triangles.emplace_back();
		triangles.back().init(world, pos, vert1, vert2, vert3, color, fixed, dynamic, density, friction, angle);
	}

	//boxes
	glm::vec2 dims;
	YabeEngine::GTex texture;
	std::string texPath;
	fload >> count;
	for (unsigned int i = 0; i < count; i++)
	{
		fload >> pos.x >> pos.y >> dims.x >> dims.y >> color.r >> color.g >> color.b >> color.a >> fixed
			>> dynamic >> texPath >> density >> friction >> angle;
		texture = YabeEngine::ResourceAgent::getTexture(texPath);
		boxes.emplace_back();
		boxes.back().init(world, pos, dims, color, fixed, dynamic, texture, density, friction, angle);
	}

	//TODO: update exit
	fload >> pos.x >> pos.y;
	levelGate = pos;

	//scale
	float scale;
	fload >> scale;
	camera.setScale(scale);
}
