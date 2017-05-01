#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "engine.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
using namespace std;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

#include <Box2D/Box2D.h>

// TODO: Modify these classes to keep track of the corresponding Box2D
// bodies. Feel free to add helpful methods like
//     mat4 getTransformation()
// and
//     vec2 getLocalPoint(vec2 worldPoint).

class Circle {
public:
    vec2 center;
    float radius;
	float angle;
	b2Body* body;
	b2Fixture *fixture;
	
    Circle() {}
    Circle(vec2 center, float radius, b2World *world, b2BodyType BodyType, float angle) {
        this->center = center;
		this->radius = radius;
		this->angle  = angle;

		b2BodyDef bodydef;
		bodydef.position.Set(center.x, center.y);
		bodydef.angle = angle;
		bodydef.type = BodyType;
		
		body = world->CreateBody(&bodydef);

		b2CircleShape shape;
		shape.m_p.Set(center.x, center.y);
		shape.m_radius = radius;

		b2FixtureDef fixturedef;
		fixturedef.shape = &shape;
		fixturedef.density = (BodyType == b2_staticBody) ? 0.0 : 1.0;
		fixture = body->CreateFixture(&fixturedef);

    }
    bool contains(vec2 worldPoint) {
        return glm::length(worldPoint - center) <= radius;
    }
	mat4 getTransformation() {
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle();
		return getMatrix()*glm::translate(vec3(pos.x, pos.y, 0))*glm::rotate(angle, vec3(0, 0, 1));
	}
    void destroy() {}
};

class Box {
public:
    vec2 center;
    vec2 size;
	float angle;
	b2Body* body;
	b2Fixture *fixture;

	Box() {}
    Box(vec2 center, vec2 size, b2World *world, b2BodyType BodyType, float angle) {
        this->center = center;
        this->size = size;
		this->angle = angle;

		b2BodyDef bodydef;
		bodydef.position.Set(center.x, center.y);
		bodydef.angle = angle;
		bodydef.type = BodyType;
		
		body = world->CreateBody(&bodydef);

		b2PolygonShape shape;
		shape.SetAsBox(size.x / 2, size.y / 2, b2Vec2(center.x, center.y), 0.0);

		b2FixtureDef fixturedef;
		fixturedef.shape = &shape;
		fixturedef.density = (BodyType == b2_staticBody) ? 0.0 : 1.0;
		fixture = body->CreateFixture(&fixturedef);
    }
    bool contains(vec2 worldPoint) {
        vec2 d = worldPoint - center;
        return (abs(d.x) <= size.x/2 && abs(d.y) <= size.y/2);
    }
	mat4 getTransformation() {
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle();
		return getMatrix()*glm::translate(vec3(pos.x, pos.y, 0))*glm::rotate(angle, vec3(0, 0, 1));
	}
    void destroy() {}
};

class Polyline {
public:
    vector<vec2> vertices;
	b2Body* body;
	b2Fixture *fixture;

	Polyline() {}
    Polyline(vector<vec2> vertices, b2World* world, b2BodyType BodyType) {
        this->vertices = vertices;

		b2BodyDef bodydef;
		bodydef.position.SetZero();
		bodydef.type = BodyType;
		
		body = world->CreateBody(&bodydef);

		vector<b2Vec2> bv2vector;
		for (int i = 0; i < vertices.size(); i++) {
			bv2vector.push_back(b2Vec2(vertices[i].x, vertices[i].y));
		}

		b2ChainShape shape;
		shape.CreateChain(&bv2vector[0], vertices.size());

		b2FixtureDef fixturedef;
		fixturedef.shape = &shape;
		fixturedef.density = (BodyType == b2_staticBody) ? 0.0 : 1.0;
		fixture = body->CreateFixture(&fixturedef);
    }
	mat4 getTransformation() {
		b2Vec2 pos = body->GetPosition();
		float angle = body->GetAngle();
		return getMatrix()*glm::translate(vec3(pos.x, pos.y, 0))*glm::rotate(angle, vec3(0, 0, 1));
	}
    void destroy() {}
};

#endif
