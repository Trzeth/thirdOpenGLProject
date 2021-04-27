#pragma once
#include <optional>
#include <memory>
#include <functional>
#include <unordered_map>

template <class T>
class HandlePool
{
public:
	HandlePool() :nextHandle(0), pool(new Pool), deleter(pool) { }

	struct HandleData {
		HandleData(uint32_t handle) : handle(handle) { }
		uint32_t handle;
	};

	using Handle = std::shared_ptr<HandleData>;
	using WeakHandle = std::weak_ptr<HandleData>;
	using Pool = std::unordered_map<uint32_t, T>;

	typename Handle GetNewHandle(T&& obj);

	typename Pool::iterator begin();
	typename Pool::iterator end();

	std::optional<std::reference_wrapper<T>> Get(Handle handle);

	const static Handle InvalidHandle;
private:
	class HandleDeleter {
	public:
		HandleDeleter(const std::shared_ptr<Pool> pool) :pool(pool) { }

		void operator()(HandleData* handle)const {
			if (!this->pool.expired()) {
				this->pool.lock()->erase(handle->handle);
			}
			delete handle;
		};

		std::weak_ptr<Pool> pool;
	};

	uint32_t nextHandle;
	std::shared_ptr<Pool> pool;
	HandleDeleter deleter;
};

template<class T>
const typename HandlePool<T>::Handle HandlePool<T>::InvalidHandle(std::make_shared<typename HandlePool<T>::HandleData>(UINT32_MAX));

template<class T>
typename HandlePool<T>::Handle HandlePool<T>::GetNewHandle(T&& obj)
{
	this->pool->emplace(this->nextHandle, std::move(obj));
	HandlePool<T>::Handle handle(new HandlePool<T>::HandleData(this->nextHandle), deleter);
	this->nextHandle++;
	return handle;
}

template<class T>
typename HandlePool<T>::Pool::iterator HandlePool<T>::begin()
{
	return this->pool->begin();
}

template<class T>
typename HandlePool<T>::Pool::iterator HandlePool<T>::end()
{
	return this->pool->end();
}

template<class T>
std::optional<std::reference_wrapper<T>> HandlePool<T>::Get(Handle handle)
{
	if (handle == nullptr || handle == InvalidHandle)
		return std::optional<std::reference_wrapper<T>>();

	auto iter = this->pool->find(handle->handle);
	if (iter == this->pool->end()) {
		return std::optional<std::reference_wrapper<T>>();
	}

	return std::optional<std::reference_wrapper<T>>(iter->second);
}