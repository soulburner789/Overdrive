#include "overdrive.h"

#include <iostream>

using namespace overdrive;

class Test :
	public core::System,
	public MessageHandler<video::Window::OnResized>,
	public MessageHandler<input::Keyboard::OnKeyPress>,
	public MessageHandler<input::Mouse::OnButtonPress>,
	public MessageHandler<input::Mouse::OnMoved>,
	public MessageHandler<input::Gamepad::OnButtonPressed>,
	public MessageHandler<input::Gamepad::OnButtonReleased>,
	public MessageHandler<input::Gamepad::OnLeftStickMoved>
{
public:
	int counter = 0;

	Test() :
		System("Test")
	{
	}

	virtual void update() override {
		//std::cout << ".";

		//if (counter++ > 100)
			//mChannel.broadcast(overdrive::core::Engine::OnStop());
	}

	void operator()(const video::Window::OnResized& msg) {
		gLog << "Window resized: " << msg.mWidth << "x" << msg.mHeight;
	}

	void operator()(const input::Keyboard::OnKeyPress& kp) {
		gLog << "Keypress: " << kp.mKey;

		if (kp.mKey == GLFW_KEY_ESCAPE)
			mChannel.broadcast(core::Engine::OnStop());
	}

	void operator()(const input::Mouse::OnButtonPress& bp) {
		auto pos = bp.mMouse->getPosition();

		gLog << "Mouse press: (" << pos.first << ", " << pos.second << ") -> " << bp.mButton;
	}

	void operator()(const input::Mouse::OnMoved& mm) {
		gLog << "(" << mm.mPositionX << ", " << mm.mPositionY << ") - d(" << mm.mDeltaX << ", " << mm.mDeltaY << ")";
	}

	void operator()(const input::Gamepad::OnButtonPressed& bp) {
		using overdrive::input::Gamepad;

		gLog << "ButtonState: " << bp.mButtonState;

		if (bp.mButtonState & Gamepad::A)
			bp.mGamepad->setVibration(1.0f, 1.0f); // maximum vibration on
	}

	void operator()(const input::Gamepad::OnButtonReleased& br) {
		using overdrive::input::Gamepad;
		
		if (br.mButtonState & Gamepad::A)
			br.mGamepad->setVibration(0.0f, 0.0f); // switch vibration off
	}

	void operator()(const input::Gamepad::OnLeftStickMoved& stick) {
		gLog << "Left stick: (" << stick.mNewPosition.first << ", " << stick.mNewPosition.second << ")";
	}
};

int main() {
	gLog << "Starting Overdrive Assault";

	core::Engine engine;
	engine.add(new overdrive::video::Video);
	engine.add(new overdrive::input::Input);
	engine.add(new Test);

	engine.run();

	gLog << "Done";
}