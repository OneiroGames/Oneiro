//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "entt/entt.hpp"

#include "Oneiro/Renderer/OpenGL/Shader.hpp"

namespace oe::World
{
    class Entity;

    class World
    {
        struct WorldData
        {
            std::string Name{};
            std::string Path{};
        };

      public:
        World() = delete;
        World(const World&) = delete;

        World(const std::string& name, const std::string& path);
        ~World();

        Entity CreateEntity(const std::string& name = {});
        void DestroyEntity(Entity entity);
        Entity GetEntity(const std::string& name = {});
        entt::registry& GetEntities();
        void DestroyAllEntities();

        /**
         * \param path Path to world file without file extension (example: "path/to/file")
         * \return If world file is exists then returns true else returns false
         */
        static bool IsExists(const std::string& path);

        /**
         * \param path Path to world file without file extension (example: "path/to/file")
         * \return Returns shared ptr to world. If world failed to load then return nullptr (check with .get() func)
         */
        static std::shared_ptr<World> Load(const std::string& path);

        /**
         * \param reWrite If you need to rewrite the output file, then you must pass true
         */
        bool Save(bool reWrite);

        void UpdateEntities();

      private:
        entt::registry mRegistry{};
        WorldData mData{};
        friend class Entity;
    };
} // namespace oe::World
