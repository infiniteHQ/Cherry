#pragma once

#include "base.hpp"

#ifndef CHERRY_IDENTIFIER
#define CHERRY_IDENTIFIER

namespace Cherry
{
	class Component;

	class Identifier
	{
	public:
		Identifier() : m_IdentifierName(std::to_string(get_unique_index())) {}
		explicit Identifier(const std::string &id) : m_IdentifierName(id) {}

		explicit Identifier(std::vector<std::shared_ptr<Component>> *components_, const std::string &id) : m_IdentifierName(id)
		{
			if (components_)
			{
				m_ComponentArrayPtr = components_;
			}
		}

		Identifier(const Identifier &other)
		{
			std::lock_guard<std::mutex> lock(other.m_Mutex);
			m_IdentifierName = other.m_IdentifierName;
			m_ComponentArrayPtr = other.m_ComponentArrayPtr;
		}

		[[nodiscard]] std::string string() const
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_IdentifierName;
		}

		[[nodiscard]] std::string component_group() const
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_ComponentGroup;
		}

		[[nodiscard]] std::vector<std::shared_ptr<Component>> *component_array_ptr() const
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_ComponentArrayPtr;
		}

		void set(const std::string &id)
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_IdentifierName = id;
		}

		Identifier &operator=(const Identifier &other)
		{
			if (this != &other)
			{
				std::scoped_lock lock(m_Mutex, other.m_Mutex);
				m_IdentifierName = other.m_IdentifierName;
			}
			return *this;
		}

		Identifier operator&(const Identifier &other) const
		{
			std::scoped_lock lock(m_Mutex, other.m_Mutex);
			return Identifier(m_IdentifierName + "_AND_" + other.m_IdentifierName);
		}

		bool operator==(const Identifier &other) const
		{
			std::scoped_lock lock(m_Mutex, other.m_Mutex);
			return m_IdentifierName == other.m_IdentifierName;
		}

		static int get_unique_index()
		{
			return anonymous_index.fetch_add(1, std::memory_order_relaxed);
		}

		static void reset_unique_index()
		{
			anonymous_index.store(0, std::memory_order_relaxed);
		}

	private:
		mutable std::mutex m_Mutex;
		std::string m_IdentifierName;
		std::string m_ComponentGroup;
		std::vector<std::shared_ptr<Component>> *m_ComponentArrayPtr = nullptr;

		inline static std::atomic<int> anonymous_index{0};
	};

}

#endif // CHERRY_IDENTIFIER