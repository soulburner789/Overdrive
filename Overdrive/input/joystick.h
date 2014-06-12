#ifndef OVERDRIVE_INPUT_JOYSTICK_H
#define OVERDRIVE_INPUT_JOYSTICK_H

#include <string>
#include <vector>

namespace overdrive {
	namespace input {
		class Joystick {
		public:
			// Signals
			struct OnMove {
				int mJoystickID;

				std::vector<float> mPosition;
				std::vector<float> mDelta;
			};

			struct OnButtonPress {
				int mJoystickID;
				int mButtonID;
			};

			struct OnButtonRelease {
				int mJoystickID;
				int mButtonID;
			};

			struct OnConnect {
				int mJoystickID;
			};

			struct OnDisconnect {
				int mJoystickID;
			};

			Joystick(int id, float deadZone = 0.05f); // the deadzone is a threshold value for broadcasting axis movement
			
			void update();

			int getJoystickID() const;
			const std::string& getJoystickName() const;

			int getNumButtons() const;
			int getNumAxes() const;

			void setDeadZone(float value);
			float getDeadZone() const;

			float getAxis(int index) const;
			const std::vector<float>& getAxes() const;

			bool isPressed(int buttonIndex) const;
			unsigned char getButton(int buttonIndex) const;
			const std::vector<unsigned char>& getButtons() const;

		private:
			int mJoystickID;

			std::string mJoystickName;
			std::vector<float> mAxisStates;
			std::vector<unsigned char> mButtonStates;

			float mDeadZone;
		};
	}
}

#endif
