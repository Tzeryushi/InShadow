#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

void Triangle::init(b2World * world, const glm::vec2 & pos,glm::vec2 vert1, glm::vec2 vert2, glm::vec2 vert3, YabeEngine::Color color, 
	bool fixed, bool dynamic, float density, float friction, float angle)
{
	//standard body init
	b2BodyDef paramBody;
	paramBody.position.Set(pos.x, pos.y);
	_color = color;

	_fixed = fixed;
	paramBody.fixedRotation = fixed;
	_dynamic = dynamic;
	if (dynamic)
		paramBody.type = b2_dynamicBody;
	else
		paramBody.type = b2_staticBody;
	paramBody.angle = angle;
	
	_body = world->CreateBody(&paramBody);
	
	//triangle specific - set 3 verts
	_vertices[0].Set(vert1.x, vert1.y);
	_vertices[1].Set(vert2.x, vert2.y);
	_vertices[2].Set(vert3.x, vert3.y);

	b2PolygonShape triShape;
	triShape.Set(_vertices, 3);

	//create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &triShape;
	fixtureDef.density = density;
	_density = density;
	fixtureDef.friction = friction;
	_friction = friction;
	_fixture = _body->CreateFixture(&fixtureDef);
}

void Triangle::draw(YabeEngine::ShapeRenderer & renderer)
{
	//get "world" coords for points
	//these need to be rotated
	std::vector <glm::vec2> vertices;
	glm::vec2 add;

	glm::vec2 midpoint;

	midpoint.x = _body->GetPosition().x;
	midpoint.y = _body->GetPosition().y;

	for (int i = 0; i < 3; i++)
	{
		add.x = _vertices[i].x;
		add.y = _vertices[i].y;
		vertices.push_back(add);
	}


	//send "world" verts to shape renderer
	renderer.drawTriangle(midpoint, vertices, _color, _body->GetAngle());
}

void Triangle::calculateShadow(glm::vec2 lightPos, float intensity)
{
	float normalAngle;

	float bestLeft = 0, bestRight = 0;
	int left = -1, right = -1;
	glm::vec2 leftVec; glm::vec2 rightVec;

	float angle = _body->GetAngle();
	glm::vec2 midpoint(_body->GetPosition().x, _body->GetPosition().y);
	glm::vec2 lightToPoint = glm::normalize(midpoint - lightPos);
	//glm::vec2 pointNormal(lightToPoint.y, -lightToPoint.x);
	for (int i = 0; i < 3; i++)
	{
		//get vertex in shape space
		glm::vec2 focusVertex(_vertices[i].x, _vertices[i].y);
		focusVertex = glm::rotate(focusVertex, angle);

		//find vector to the worldspace vert, get angle between
		glm::vec2 lightToVert = glm::normalize(focusVertex + midpoint - lightPos);
		normalAngle = glm::orientedAngle(lightToPoint, lightToVert);

		if (left == -1 || right == -1)
		{
			bestLeft = normalAngle;
			left = i;
			leftVec = glm::rotate(lightToVert, -angle);
			bestRight = normalAngle;
			right = i;
			rightVec = glm::rotate(lightToVert, -angle);
		}
		else if (normalAngle < bestLeft)
		{
			bestLeft = normalAngle;
			left = i;
			leftVec = glm::rotate(lightToVert, -angle);
		}
		else if (normalAngle > bestRight)
		{
			bestRight = normalAngle;
			right = i;
			rightVec = glm::rotate(lightToVert, -angle);
		}
	}

	//creating shape space pass back
	std::vector<glm::vec4> passVerts;
	_shadowVecs.clear();

	passVerts.emplace_back(midpoint.x, midpoint.y, _vertices[left].x, _vertices[left].y);
	leftVec = glm::vec2(_vertices[left].x, _vertices[left].y) + (leftVec * intensity);
	passVerts.emplace_back(midpoint.x, midpoint.y, leftVec.x, leftVec.y);

	passVerts.emplace_back(midpoint.x, midpoint.y, _vertices[right].x, _vertices[right].y);
	rightVec = glm::vec2(_vertices[right].x, _vertices[right].y) + (rightVec * intensity);
	passVerts.emplace_back(midpoint.x, midpoint.y, rightVec.x, rightVec.y);

	//update privates for renewal
	_shadowAngle = angle;
	_shadowMid = midpoint;
	for (int i = 0; i < 4; i++)
		_shadowVecs.emplace_back(passVerts[i].z, passVerts[i].w);
	_redShadow = false;
}

void Triangle::drawShadow(YabeEngine::ShapeRenderer & renderer)
{
	if (_redShadow)
		renderer.drawQuad(_shadowMid, _shadowVecs, YabeEngine::Color(190, 0, 0, 170), _shadowAngle);
	renderer.drawQuad(_shadowMid, _shadowVecs, YabeEngine::Color(0, 0, 0, 170), _shadowAngle);
}

void Triangle::drawShadow(YabeEngine::Color color, YabeEngine::ShapeRenderer & renderer)
{
	if (_redShadow)
		renderer.drawQuad(_shadowMid, _shadowVecs, YabeEngine::Color(190, 0, 0, 170), _shadowAngle);
	renderer.drawQuad(_shadowMid, _shadowVecs, color, _shadowAngle);
}

void Triangle::drawHardShadow(YabeEngine::ShapeRenderer & renderer)
{
	if (!_shadow.isInit())
		return;
	_shadow.draw(renderer);
}

void Triangle::destruct(b2World * world)
{
	world->DestroyBody(_body);
	if (_shadow.isInit())
		_shadow.destruct(world);
}

std::vector<glm::vec2> Triangle::getVertices()
{
	std::vector<glm::vec2> tempPass;
	tempPass.emplace_back(_vertices[0].x, _vertices[0].y);
	tempPass.emplace_back(_vertices[1].x, _vertices[1].y);
	tempPass.emplace_back(_vertices[2].x, _vertices[2].y);
	return tempPass;
}

int Triangle::renewShadow(b2World* world)
{
	if (_redShadow)
		return 0;
	if (_shadowVecs.empty() || _shadowVecs.size() < 4)
		return -1;
	if (_shadow.isInit())
		_shadow.destruct(world);
	Quad newQuad;
	newQuad.init(world, _shadowMid, _shadowVecs, YabeEngine::Color(0, 0, 0, 255), false, false, 1.0f, 0.3f, _shadowAngle);
	_shadow = newQuad;
	return 1;
}

void Triangle::destroyShadow(b2World * world)
{
	if (_shadow.isInit())
		_shadow.destruct(world);
}

bool Triangle::checkPoint(glm::vec2 pos)
{
	std::vector <glm::vec2> transformed;

	glm::vec2 altV1(_shadowVecs[0].x, _shadowVecs[0].y);
	glm::vec2 altV2(_shadowVecs[1].x, _shadowVecs[1].y);
	glm::vec2 altV3(_shadowVecs[2].x, _shadowVecs[2].y);
	glm::vec2 altV4(_shadowVecs[3].x, _shadowVecs[3].y);

	altV1 = glm::rotate(altV1, _shadowAngle);
	altV2 = glm::rotate(altV2, _shadowAngle);
	altV3 = glm::rotate(altV3, _shadowAngle);
	altV4 = glm::rotate(altV4, _shadowAngle);

	//I store my quads like Box2D. need to switch to linear order to get them to work right
	transformed.emplace_back(_shadowMid.x + altV1.x, _shadowMid.y + altV1.y);
	transformed.emplace_back(_shadowMid.x + altV2.x, _shadowMid.y + altV2.y);
	transformed.emplace_back(_shadowMid.x + altV4.x, _shadowMid.y + altV4.y);
	transformed.emplace_back(_shadowMid.x + altV3.x, _shadowMid.y + altV3.y);

	int i, j;
	bool check = false;
	for (i = 0, j = 3; i < 4; j = i++) {
		if (((transformed[i].y>pos.y) != (transformed[j].y>pos.y)) &&
			(pos.x < (transformed[j].x - transformed[i].x) * (pos.y - transformed[i].y) / (transformed[j].y - transformed[i].y) + transformed[i].x))
			check = !check;
	}

	_redShadow = check;
	return check;
}
