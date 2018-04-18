#pragma once
#include "Filters.h"

#include "../Util/Function.h"

#include <Box2D/Box2D.h>
#include <graphics/material.h>
#include <graphics/mesh_instance.h>

// Container of properties for setting up a game object
struct GOProperties
{
	b2BodyType type			= b2_dynamicBody;
	b2Vec2 position			= { 0.f, 0.f };
	float32 angle			= 0.f;
	float32 friction		= 1.f;
	float32 restitution		= 0.f;
	float32 density			= 1.f;
	float32 gravityScale	= 1.f;
	bool isSensor			= false;
	bool fixedRotation		= false;
	uint16 categoryBits		= CATEGORY_DEFAULT;
	uint16 maskBits			= MASK_ALL;
	const gef::Mesh* mesh	= nullptr;
	gef::Vector4 size		= { 1.f, 1.f, 1.f };
};

namespace gef
{
	class Renderer3D;
}

class GameObject
{
public:
	using BodyGenFunc = Function<b2Body*(GameObject*)>;

	GameObject()			= default;
	virtual ~GameObject()	= default;

	GameObject(b2World* world, const GOProperties& properties);
	void init(b2World* world, const GOProperties& properties);

	// This constructor is used for objects whose physics bodies
	// are more complex than can be described through GOProperties.
	explicit GameObject(BodyGenFunc genBody);
	void init(BodyGenFunc genBody);
	void update();

	void destroy(b2World* world);

	void render(gef::Renderer3D* renderer) const;

	void setAngularVelocity(float velocity);
	void setVelocity(const b2Vec2& velocity);
	void setXVelocity(float x);
	void setYVelocity(float y);

	void setPosition(const b2Vec2& position);
	void setX(float x) { setPosition({ x, m_body->GetPosition().y }); }
	void setY(float y) { setPosition({ m_body->GetPosition().x, y }); }

	void setActive(bool val) { m_body->SetActive(val); }
	void setFilters(UInt16 mask, UInt16 category);
	void setSensor(bool val);

	void setColour(UInt32 colour) { m_material.set_colour(colour); }
	void setTexture(gef::Texture* texture) { m_material.set_texture(texture); }

	b2Vec2 getVelocity() const { return m_body->GetLinearVelocity(); }
	gef::Vector4 getSize() const { return m_instance.transform().GetScale(); }
	b2Vec2 getPositionVec2() const { return m_body->GetPosition(); }
	gef::Vector4 getPositionVec4() const { return m_instance.transform().GetTranslation(); }

	enum class Type : char
	{
		Player,
		Platform,
		Powerup,
		Floor,
		Boundary,
		NA
	};

	// Used for objects that are mostly just there for
	// collision detection (like the floor) and don't need their own class
	void overrideType(Type type) { m_type = type; }

	Type type() const { return m_type; }

protected:
	Type m_type = Type::NA;
	b2Body* m_body = nullptr;

private:
	gef::Material m_material;
	gef::MeshInstance m_instance;
};