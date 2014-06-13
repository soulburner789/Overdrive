#include "input/input.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/joystick.h"
#include "core/engine.h"
#include "video/video.h"
#include "opengl.h"

namespace overdrive {
	namespace input {
		Input::Input():
			System("Input")
		{
			for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i)
				mJoysticks.emplace_back(Joystick(-1));
		}

		bool Input::initialize() {
			System::initialize();

			mEngine->updateSystem(this, true, false); 

			auto vid = mEngine->get("Video");
			
			if (vid) {
				auto vidSystem = static_cast<overdrive::video::Video*>(vid);

				for (auto& w : vidSystem->getWindows()) {
					addKeyboard(Keyboard(&w));
					addMouse(Mouse(&w));
				}
			}

			return true;
		}

		void Input::update() {
			//continually see if joysticks have connected/disconnected
			for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
				if (glfwJoystickPresent(i)) {
					if (mJoysticks[i].getJoystickID() == -1) {
						mJoysticks[i] = Joystick(i);
						core::Channel::broadcast(Joystick::OnConnect{ i, glfwGetJoystickName(i) });
					}

					mJoysticks[i].update();
				}
				else {
					if (mJoysticks[i].getJoystickID() != -1) {
						core::Channel::broadcast(Joystick::OnDisconnect{ i });
						mJoysticks[i] = Joystick(-1);
					}
				}
			}

			glfwPollEvents();
		}

		void Input::shutdown() {
			System::shutdown();
		}

		void Input::addKeyboard(Keyboard&& kb) {
			mKeyboards.emplace_back(std::move(kb));
		}

		void Input::addMouse(Mouse&& m) {
			mMice.emplace_back(std::move(m));
		}

		void Input::operator()(const video::Window::OnCreate& onCreate) {
			addKeyboard(Keyboard(onCreate.window));
			addMouse(Mouse(onCreate.window));
		}

		void Input::operator()(const video::Window::OnClose& onClose) {
			unregisterKeyboard(onClose.window);
			unregisterMouse(onClose.window);

			auto it = std::remove_if(
				mKeyboards.begin(), 
				mKeyboards.end(), 
				[onClose](const Keyboard& kb) { 
					return kb.isAssociatedWith(onClose.window); 
				}
			);

			if (it != mKeyboards.end())
				mKeyboards.erase(it);

			auto jt = std::remove_if(
				mMice.begin(),
				mMice.end(),
				[onClose](const Mouse& m) {
					return m.isAssociatedWith(onClose.window);
				}
			);

			if (jt != mMice.end())
				mMice.erase(jt);
		}
	}
}