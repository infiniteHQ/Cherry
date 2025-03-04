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
		Identifier() : m_IdentifierName(generate_id()) {}
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

		[[nodiscard]] std::vector<std::shared_ptr<Component>> *component_array_ptr() const
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_ComponentArrayPtr;
		}

		[[nodiscard]] std::string component_group() const
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			return m_ComponentGroup;
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
				m_ComponentArrayPtr = other.m_ComponentArrayPtr;
				incrementor_level = other.incrementor_level;
			}
			return *this;
		}

		bool operator==(const Identifier &other) const
		{
			std::scoped_lock lock(m_Mutex, other.m_Mutex);
			return m_IdentifierName == other.m_IdentifierName;
		}

		static void UpgradeIncrementorLevel()
		{
			std::lock_guard<std::mutex> lock(level_mutex);
			incrementor_level++;
			level_indices.emplace_back(0);
		}

		static void DowngradeIncrementorLevel()
		{
			std::lock_guard<std::mutex> lock(level_mutex);
			if (incrementor_level > 1)
			{
				incrementor_level--;
			}
		}

		static std::string GetUniqueIndex()
		{
			int level = incrementor_level;
			level--; // The incrementor start from one, memory start from zero, we need to decrement it to adapt the memory managment

			std::lock_guard<std::mutex> lock(level_mutex);

			if (level < level_indices.size())
			{
				level_indices[level]++;
			}
			else
			{
				while (level_indices.size() <= level)
				{
					level_indices.push_back(0);
				}
				level_indices[level] = 1;
			}

			return generate_id();
		}

		static void ResetUniqueIndex()
		{
			std::lock_guard<std::mutex> lock(level_mutex);
			incrementor_level = 1;
			level_indices = {0};
		}

		static std::shared_ptr<Component> GetComponent()
		{
			return nullptr; // TODO
		}

	private:
		mutable std::mutex m_Mutex;
		std::string m_IdentifierName;
		std::vector<std::shared_ptr<Component>> *m_ComponentArrayPtr = nullptr;
		std::string m_ComponentGroup;
		inline static std::mutex level_mutex;
		inline static int incrementor_level = 1;
		inline static std::vector<int> level_indices = {0};

		static std::string generate_id()
		{
			int level = incrementor_level;
			std::string id;
			for (size_t i = 0; i < level_indices.size(); ++i)
			{
				if (i >= level)
					continue;

				if (i % 2 == 0)
					id += std::to_string(level_indices[i]);
				else
					id += convert_to_letters(level_indices[i]);
			}
			return id;
		}

		static std::string convert_to_letters(int index)
		{
			std::string result;
			while (index >= 0)
			{
				result.insert(result.begin(), 'a' + (index % 26));
				index = (index / 26) - 1;
			}
			return result;
		}
	};
}

using CherryID = Cherry::Identifier;

#endif // CHERRY_IDENTIFIER