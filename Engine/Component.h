#pragma once

#include <memory>

namespace nu {
	class Actor;
	class Renderer;

	class Component {
	public:
		virtual ~Component() = default;

		// Clone for deep-copying components
		virtual std::unique_ptr<Component> Clone() const = 0;

		// Lifecycle
		virtual void OnAttach(Actor& actor) {}
		virtual void OnDetach(Actor& actor) {}
		virtual void Update(Actor& actor, float dt) {}
		virtual void Draw(const Renderer& renderer, const Actor& actor) const {}
	};
}
