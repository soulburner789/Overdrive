#pragma once

#include <vector>
#include "../core/system.h"
#include "../video/window.h"
#include "gamepad.h"

namespace overdrive {
	namespace input {
		class Keyboard;
		class Mouse;

		class Input:
			public core::System,
			public MessageHandler<video::Window::OnCreated>,
			public MessageHandler<video::Window::OnClosed>
		{
		public:
			Input();

			virtual void initialize() override;
			virtual void update() override;
			virtual void shutdown() override;

			const std::vector<Keyboard*>& getKeyboardList() const;
			const std::vector<Mouse*>& getMouseList() const;
			const std::vector<Gamepad>& getGamepadList() const;

			void operator()(const video::Window::OnCreated& created);
			void operator()(const video::Window::OnClosed& closed);

		private:
			std::vector<Keyboard*> mKeyboardList;
			std::vector<Mouse*> mMouseList;
			std::vector<Gamepad> mGamepadList;
		};

		namespace detail {
			void loadXInput();
			void unloadXInput();
		}
	}
}
