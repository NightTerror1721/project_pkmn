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
	wref<_Ty> getGameObjectById(UniqueId uid)
	{
		auto it = _objs.find(uid);
		return it == _objs.end() ? nullptr : it->second;
	}

	void forEachGameObject(const std::function<void(wref<_Ty>)>& consumer)
	{
		for (auto& obj : _objs)
			consumer(obj->second);
	}

	void forEachGameObject(const std::function<void(const wref<_Ty>)>& consumer) const
	{
		for (const auto& obj : _objs)
			consumer(obj->second);
	}

	bool destroyGameObject(UniqueId uid)
	{
		auto it = _objs.find(uid);
		if (it != _objs.end())
		{
			it->reset();
			_objs.erase(it);
			return true;
		}
		return false;
	}

	wref<_Ty> addGameObject(wref<_Ty> obj)
	{
		UniqueId uid = obj.uid();
		auto it = _objs.find(uid);
		if (it != _objs.end())
			return nullptr;

		//_objs[uid] = std::make_shared()
	}

public:
	class iterator;
	class const_iterator;

	class iterator
	{
	private:
		decltype(_objs)::iterator _it;

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = wref<_Ty>;
		using pointer = ref<_Ty>*;
		using reference = wref<_Ty>;

		iterator(const decltype(_objs)::iterator& it) : _it{ it } {}

		inline reference operator* () { return *_it; }
		inline pointer operator-> () { return std::addressof(*_it); }

		iterator& operator++ () { return ++_it, *this; }
		iterator& operator++ (int) { return _it++, *this; }

		inline bool operator== (const iterator& right) const { return _it == right._it; }
		inline bool operator!= (const iterator& right) const { return _it != right._it; }
	};

	class const_iterator
	{
	private:
		decltype(_objs)::const_iterator _it;

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = const wref<_Ty>;
		using pointer = const ref<_Ty>*;
		using reference = const wref<_Ty>;

		const_iterator(const decltype(_objs)::const_iterator& it) : _it{ it } {}
		const_iterator(const iterator& it) : _it{ it._it } {}

		inline reference operator* () { return *_it; }
		inline pointer operator-> () { return std::addressof(*_it); }

		const_iterator& operator++ () { return ++_it, *this; }
		const_iterator& operator++ (int) { return _it++, *this; }

		inline bool operator== (const const_iterator& right) const { return _it == right._it; }
		inline bool operator!= (const const_iterator& right) const { return _it != right._it; }

		inline bool operator== (const iterator& right) const { return _it == right._it; }
		inline bool operator!= (const iterator& right) const { return _it != right._it; }

		friend bool operator== (const iterator& left, const const_iterator& right) const { return const_iterator(left) == right; }
		friend bool operator!= (const iterator& left, const const_iterator& right) const { return const_iterator(left) != right; }
	};

	inline iterator begin() { return iterator(_objs.begin()); }
	inline iterator begin() const { return iterator(_objs.begin()); }
	inline iterator cbegin() const { return iterator(_objs.cbegin()); }

	inline iterator end() { return iterator(_objs.end()); }
	inline iterator end() const { return iterator(_objs.end()); }
	inline iterator cend() const { return iterator(_objs.cend()); }
};

