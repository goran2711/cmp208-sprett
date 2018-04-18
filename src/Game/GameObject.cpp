#include "GameObject.h"
#include <graphics/renderer_3d.h>

GameObject::GameObject(BodyGenFunc genBody)
	:	m_body(genBody(this))
{
}

GameObject::GameObject(b2World* world, const GOProperties& properties)
{
	init(world, properties);
}

void GameObject::init(BodyGenFunc genBody)
{
	m_body = genBody(this);
}

void GameObject::destroy(b2World* world)
{
	world->DestroyBody(m_body);
}

void GameObject::init(b2World * world, const GOProperties & properties)
{
	b2BodyDef def;
	def.type = properties.type;
	def.position = properties.position;
	def.angle = properties.angle;
	def.gravityScale = properties.gravityScale;
	def.fixedRotation = properties.fixedRotation;
	def.userData = this;

	m_body = world->CreateBody(&def);

	b2PolygonShape shape;
	shape.SetAsBox(properties.size.x() * 0.5f, properties.size.y() * 0.5f);

	b2FixtureDef fix;
	fix.shape = &shape;
	fix.restitution = properties.restitution;
	fix.friction = properties.friction;
	fix.isSensor = properties.isSensor;
	fix.density = properties.density;
	
	b2Filter filter;
	filter.categoryBits = properties.categoryBits;
	filter.maskBits = properties.maskBits;
	fix.filter = filter;

	m_body->CreateFixture(&fix);

	m_instance.set_mesh(properties.mesh);

	// Set initial scale (it is reused in GameObject::updateFromSimulation)
	gef::Matrix44 transform;
	transform.Scale(properties.size);
	m_instance.set_transform(transform);

	update();
}

void GameObject::update()
{
	gef::Matrix44 scale;
	scale.Scale(m_instance.transform().GetScale());

	gef::Matrix44 rotation;
	rotation.RotationZ(m_body->GetAngle());

	gef::Matrix44 transform = scale * rotation;

	gef::Vector4 translation(m_body->GetPosition().x, m_body->GetPosition().y, 0.0f);
	transform.SetTranslation(translation);

	m_instance.set_transform(transform);
}

void GameObject::render(gef::Renderer3D* renderer) const
{
	renderer->set_override_material(&m_material);
	renderer->DrawMesh(m_instance);
	renderer->set_override_material(nullptr);
}

void GameObject::setAngularVelocity(float velocity)
{
	m_body->SetAngularVelocity(velocity);
}

void GameObject::setVelocity(const b2Vec2& velocity)
{
	m_body->SetLinearVelocity(velocity);
}

void GameObject::setXVelocity(float x)
{
	setVelocity({ x, m_body->GetLinearVelocity().y });
}

void GameObject::setYVelocity(float y)
{
	setVelocity({ m_body->GetLinearVelocity().x, y });
}

void GameObject::setPosition(const b2Vec2& position)
{
	auto transform = m_instance.transform();
	auto translation = transform.GetTranslation();
	translation.set_x(position.x);
	translation.set_y(position.y);
	transform.SetTranslation(translation);

	m_instance.set_transform(transform);
	m_body->SetTransform(position, 0.f);
}

void GameObject::setFilters(UInt16 mask, UInt16 category)
{
	b2Filter filter;
	filter.maskBits = mask;
	filter.categoryBits = category;

	auto fixture = m_body->GetFixtureList();

	while (fixture)
	{
		fixture->SetFilterData(filter);

		fixture = fixture->GetNext();
	}
}

void GameObject::setSensor(bool val)
{
	auto fixture = m_body->GetFixtureList();

	while (fixture)
	{
		fixture->SetSensor(val);

		fixture = fixture->GetNext();
	}
}