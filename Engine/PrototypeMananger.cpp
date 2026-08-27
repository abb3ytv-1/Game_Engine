#include "pch.h"
#include "PrototypeManager.h"

#include "PlayerComponent.h"
#include "RigidBodyComponent.h"
#include "BulletComponent.h"
#include "EnemyAIComponent.h"
#include "CircleCollider2DComponent.h"

#include <iostream>

namespace nu
{
    bool PrototypeManager::Load(
        const rapidjson::Value& prototypes)
    {
        if (!prototypes.IsObject())
        {
            std::cerr
                << "Prototype data is not an object."
                << std::endl;

            return false;
        }

        a_prototypes.CopyFrom(
            prototypes,
            a_prototypes.GetAllocator()
        );

        std::cout
            << "Loaded "
            << a_prototypes.MemberCount()
            << " prototypes."
            << std::endl;

        return true;
    }


    std::unique_ptr<Actor>
        PrototypeManager::Instantiate(
            const std::string& name) const
    {
        if (!a_prototypes.HasMember(name.c_str()))
        {
            std::cerr
                << "Prototype not found: "
                << name
                << std::endl;

            return nullptr;
        }

        const rapidjson::Value& prototype =
            a_prototypes[name.c_str()];

        if (!prototype.IsObject())
        {
            std::cerr
                << "Prototype is not an object: "
                << name
                << std::endl;

            return nullptr;
        }

        auto actor =
            std::make_unique<Actor>();


        // -------------------------------------------------
        // Tag
        // -------------------------------------------------

        if (prototype.HasMember("tag") &&
            prototype["tag"].IsString())
        {
            actor->SetTag(
                prototype["tag"].GetString()
            );
        }


        // -------------------------------------------------
        // Lifespan
        // -------------------------------------------------

        if (prototype.HasMember("lifespan") &&
            prototype["lifespan"].IsNumber())
        {
            actor->SetLifespan(
                prototype["lifespan"].GetFloat()
            );
        }


        // -------------------------------------------------
        // Transform
        // -------------------------------------------------

        if (prototype.HasMember("transform") &&
            prototype["transform"].IsObject())
        {
            const rapidjson::Value& transform =
                prototype["transform"];

            Vector2 position{
                0.0f,
                0.0f
            };

            float rotation = 0.0f;
            float scale = 1.0f;

            if (transform.HasMember("position") &&
                transform["position"].IsArray() &&
                transform["position"].Size() == 2 &&
                transform["position"][0].IsNumber() &&
                transform["position"][1].IsNumber())
            {
                position.x =
                    transform["position"][0].GetFloat();

                position.y =
                    transform["position"][1].GetFloat();
            }

            if (transform.HasMember("rotation") &&
                transform["rotation"].IsNumber())
            {
                rotation =
                    transform["rotation"].GetFloat();
            }

            if (transform.HasMember("scale") &&
                transform["scale"].IsNumber())
            {
                scale =
                    transform["scale"].GetFloat();
            }

            actor->SetTransform(
                Transform{
                    position,
                    rotation,
                    scale
                }
            );
        }


        // -------------------------------------------------
        // Components
        // -------------------------------------------------

        if (prototype.HasMember("components") &&
            prototype["components"].IsArray())
        {
            const rapidjson::Value& components =
                prototype["components"];

            for (rapidjson::SizeType i = 0;
                i < components.Size();
                ++i)
            {
                const rapidjson::Value& component =
                    components[i];

                if (!component.IsObject())
                {
                    std::cerr
                        << "Invalid component in prototype: "
                        << name
                        << std::endl;

                    continue;
                }

                if (!component.HasMember("type") ||
                    !component["type"].IsString())
                {
                    std::cerr
                        << "Component is missing a type in prototype: "
                        << name
                        << std::endl;

                    continue;
                }

                std::string type =
                    component["type"].GetString();


                // -------------------------------------------------
                // Player
                // -------------------------------------------------

                if (type == "Player")
                {
                    float speed = 300.0f;
                    int ammo = 0;

                    if (component.HasMember("speed") &&
                        component["speed"].IsNumber())
                    {
                        speed =
                            component["speed"].GetFloat();
                    }

                    if (component.HasMember("ammo") &&
                        component["ammo"].IsInt())
                    {
                        ammo =
                            component["ammo"].GetInt();
                    }

                    actor->AddComponent(
                        std::make_unique<PlayerComponent>(
                            speed,
                            ammo
                        )
                    );
                }


                // -------------------------------------------------
                // Enemy AI
                // -------------------------------------------------

                else if (type == "EnemyAI")
                {
                    float speed = 100.0f;

                    if (component.HasMember("speed") &&
                        component["speed"].IsNumber())
                    {
                        speed =
                            component["speed"].GetFloat();
                    }

                    actor->AddComponent(
                        std::make_unique<EnemyAIComponent>(
                            nullptr,
                            speed
                        )
                    );
                }


                // -------------------------------------------------
                // RigidBody
                // -------------------------------------------------

                else if (type == "RigidBody")
                {
                    actor->AddComponent(
                        std::make_unique<RigidBodyComponent>()
                    );
                }


                // -------------------------------------------------
                // Bullet
                // -------------------------------------------------

                else if (type == "Bullet")
                {
                    actor->AddComponent(
                        std::make_unique<BulletComponent>()
                    );
                }


                // -------------------------------------------------
                // Collider
                // -------------------------------------------------

                else if (type == "CircleCollider")
                {
                    float radius = 1.0f;

                    if (component.HasMember("radius") &&
                        component["radius"].IsNumber())
                    {
                        radius =
                            component["radius"].GetFloat();
                    }

                    actor->AddComponent(
                        std::make_unique<
                        CircleCollider2DComponent
                        >(radius)
                    );
                }


                // -------------------------------------------------
                // Unknown
                // -------------------------------------------------

                else
                {
                    std::cerr
                        << "Unknown component type: "
                        << type
                        << " in prototype: "
                        << name
                        << std::endl;
                }
            }
        }

        std::cout
            << "Instantiated prototype: "
            << name
            << std::endl;

        return actor;
    }
}