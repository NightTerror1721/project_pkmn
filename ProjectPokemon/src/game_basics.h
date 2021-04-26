#pragma once

#include "common.h"

class GameController;

struct Renderable
{
	virtual void render(sf::RenderTarget& canvas) = 0;
};

struct Updatable
{
	virtual void update(const sf::Time& delta) = 0;
};

struct EventDispatcher
{
	virtual void dispatchEvent(const sf::Event& event) = 0;
};

class GameObject : public Renderable, public Updatable, public EventDispatcher
{
private:
	UniqueId _uid = UniqueId::make();
	GameController* _gc = nullptr;

public:
	GameObject() = default;
	GameObject(const GameObject&) = default;
	GameObject(GameObject&&) noexcept = default;
	~GameObject() = default;

	GameObject& operator= (const GameObject&) = default;
	GameObject& operator= (GameObject&&) noexcept = default;

	inline UniqueId uid() const { return _uid; }

public:
	virtual void render(sf::RenderTarget& canvas) override {}
	virtual void update(const sf::Time& delta) override {}
	virtual void dispatchEvent(const sf::Event& event) override {}

public:
	inline GameController& getGameController() const { return *_gc; }
	inline bool isAttachedToGameController() const { return _gc; }

public:
	friend class GameController;
};



template<std::derived_from<GameObject> _Ty>
class GameObjectContainer
{
private:
	std::unordered_map<UniqueId, ref<_Ty>, UniqueId::hash> _objs;

public:
	GameObjectContainer() = default;
	GameObjectContainer(GameObjectContainer&&) noexcept = default;
	~GameObjectContainer() = default;

	GameObjectContainer& operator=(GameObjectContainer&&) noexcept = default;

	GameObjectContainer(const GameObjectContainer&) = delete;
	GameObjectContainer& operator= (const GameObjectContainer&) = delete;

public:
	ref<_Ty> getById(UniqueId uid)
	{
		auto it = _objs.find(uid);
		return it == _objs.end() ? nullptr : it->second;
	}



};
