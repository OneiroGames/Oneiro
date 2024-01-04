//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Common.hpp"
#include "Oneiro/Common/EngineApi.hpp"
#include "Oneiro/Common/FileSystem/FileSystem.hpp"
#include "Oneiro/Common/World/Components/Components.hpp"

#include "nameof.hpp"

namespace oe
{
	class World;

	class Entity
	{
	public:
		Entity() = default;

		Entity(flecs::entity handle, World* world) : m_Handle(handle), m_World(world)
		{
			if (IsValid())
				GetOrAddComponent<TransformComponent>();
		}

		Entity CreateChild(const std::string& name)
		{
			if (!IsValid())
			{
				OE_CORE_WARN("Invalid entity in function {}", NAMEOF(CreateChild(name)).c_str());
				return {};
			}

			return {m_Handle, nullptr};
		}

		template <class T>
		[[nodiscard]] bool HasComponent() const
		{
			if (!IsValid())
			{
				OE_CORE_WARN("Invalid entity in function {}", NAMEOF(HasComponent<T>()).c_str());
				return {};
			}

			return m_Handle.has<T>();
		}

		template <class T, class... Args>
		T* AddComponent(const Args&... args)
		{
			if (!IsValid())
			{
				OE_CORE_WARN("Invalid entity in function {}", NAMEOF(AddComponent<T>(args...)).c_str());
				return {};
			}

			if (HasComponent<T>())
				return ForceGetComponent<T>();
			return m_Handle.add<T>(args...).template get_mut<T>();
		}

		template <class T>
		T* GetComponent()
		{
			if (!IsValid())
			{
				OE_CORE_WARN("Invalid entity in function {}", NAMEOF(GetComponent<T>()).c_str());
				return {};
			}

			if (!HasComponent<T>())
				return {};
			return m_Handle.get_mut<T>();
		}

		template <class T, class... Args>
		T* GetOrAddComponent(const Args&... args)
		{
			if (!IsValid())
			{
				OE_CORE_WARN("Invalid entity in function {}", NAMEOF(GetOrAddComponent<T>(args...)).c_str());
				return {};
			}

			const auto& component = GetComponent<T>();
			if (component)
				return component;
			else
				return ForceAddComponent<T>(args...);
		}

		template <class T>
		void RemoveComponent()
		{
			if (!IsValid())
			{
				OE_CORE_WARN("Invalid entity in function {}", NAMEOF(RemoveComponent<T>()).c_str());
				return;
			}

			if (!HasComponent<T>())
				return;
			m_Handle.remove<T>();
		}

		Entity ForceCreateChild(const std::string& name)
		{
			return {m_Handle, nullptr};
		}

		template <class T>
		[[nodiscard]] bool ForceHasComponent() const
		{
			return m_Handle.has<T>();
		}

		template <class T, class... Args>
		T* ForceAddComponent(const Args&... args)
		{
			return m_Handle.add<T>(args...).template get_mut<T>();
		}

		template <class T>
		T* ForceGetComponent()
		{
			return m_Handle.get_mut<T>();
		}

		template <class T>
		void ForceRemoveComponent()
		{
			m_Handle.remove<T>();
		}

		[[nodiscard]] bool IsValid() const
		{
			return m_World && m_Handle;
		}

		operator bool() const
		{
			return IsValid();
		}

		std::string GetName()
		{
			if (!IsValid())
				return {};

			return m_Handle.name().c_str();
		}

	private:
		friend class World;
		flecs::entity m_Handle{};
		World* m_World{};
	};

	class World
	{
	public:
		bool Load(const FileSystem::Path& path)
		{
			m_Path = path;
			m_Root = EngineApi::GetECS()->entity("Root");
			//			const auto node = YAML::Load(FileSystem::Read(path));
			//			const auto& entities = node["Entities"];
			//			for (const auto& entityNode : entities)
			//			{
			//				auto entity = CreateEntity(entityNode["Name"].as<std::string>());
			//
			//				entity.GetOrAddComponent<TransformComponent>()->Serialize(entityNode["TransformComponent"]);
			//			}

			return true;
		}

		bool UnLoad()
		{
			return Save();
		}

		bool Save()
		{
			//			m_Root.children([this, &out](flecs::entity children) {
			//				Entity entity{children, this};
			//				if (entity)
			//				{
			//				}
			//			});
			//			FileSystem::Write(m_Path, reinterpret_cast<const uint8_t*>(out.c_str()), out.size());

			return true;
		}

		Entity CreateEntity(const std::string& name)
		{
			return {EngineApi::GetECS()->entity(name.c_str()).child_of(m_Root), this};
		}

		Entity GetEntity(const std::string& name)
		{
			return {m_Root.lookup(name.c_str()), this};
		}

		void DestroyEntity(const std::string& name)
		{
			auto entity = GetEntity(name);
			if (entity)
				EngineApi::GetECS()->delete_with(entity.m_Handle);
		}

		void DestroyEntity(const Entity& entity)
		{
			if (entity)
				EngineApi::GetECS()->delete_with(entity.m_Handle);
		}

		bool HasEntity(const std::string& name)
		{
			return GetEntity(name);
		}

		Entity GetOrCreateEntity(const std::string& name)
		{
			auto entity = GetEntity(name);
			if (entity)
				return entity;
			else
				return CreateEntity(name);
		}

		std::vector<Entity> GetEntities()
		{
			std::vector<Entity> result{};
			EngineApi::GetECS()->each([&](flecs::entity flecsEntity) {
				result.emplace_back(flecsEntity, this);
			});
			return result;
		}

		bool UpdateRuntime(float deltaTime)
		{
			return EngineApi::GetECS()->progress(deltaTime);
		}

	private:
		flecs::entity m_Root{};
		FileSystem::Path m_Path{};
	};

	class WorldManager
	{
	public:
		World* CreateWorld()
		{
			m_CurrentWorld = CreateRef<World>();
			return m_CurrentWorld.get();
		}

		World* LoadWorld(const FileSystem::Path& path)
		{
			m_CurrentWorld = CreateRef<World>();
			m_CurrentWorld->Load(path);
			return m_CurrentWorld.get();
		}

		void UnLoadWorld()
		{
			m_CurrentWorld->UnLoad();
			m_CurrentWorld.reset();
		}

		World* GetWorld() const
		{
			return m_CurrentWorld.get();
		}

		template <class... Components, class... Args>
		flecs::system_builder<Components...> RegisterSystem(const std::string& name, const Args&... args)
		{
			return EngineApi::GetECS()->system<Components...>(name.c_str(), args...);
		}

	private:
		Ref<World> m_CurrentWorld{};
	};
} // namespace oe