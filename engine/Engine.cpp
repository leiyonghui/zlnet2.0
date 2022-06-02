#include "Engine.h"

namespace engine
{
	Engine::Engine():_runing(false), _lastClock1000ms(0), _msgqueue(new MsgQueue<PacketPtr>())
	{
	}

	Engine::~Engine()
	{
		delete _msgqueue;
	}

	void Engine::run()
	{
		_runing = true;

		onInit();

		onLoop();

		onQuit();
	}

	void Engine::bindMsgdispatcher(const std::function<void(const PacketPtr&)>& func)
	{
		_msgqueue->bindDispatcher(func);
	}

	void Engine::bindMsgdispatcher(std::function<void(const PacketPtr&)>&& func)
	{
		_msgqueue->bindDispatcher(std::move(func));
	}

	void Engine::onInit()
	{

	}

	void Engine::onQuit()
	{

	}

	void Engine::onPoll(bool& busy)
	{
		auto now = TimeHelp::clock_ms().count();

		if (now - _lastClock1000ms >= 1000)
		{
			_lastClock1000ms = now;
			try
			{
				onTimer1000ms();
			}
			catch (const std::exception& ex)
			{
				core_log_error("onTimer1000ms exception", ex.what());
			}
		}

		TimerWheel::update(now);

		_msgqueue->poll(busy);
	}

	void Engine::onIdel()
	{

	}

	void Engine::onTimer1000ms()
	{
	}

	void Engine::onLoop()
	{
		while (_runing)
		{
			bool busy = false;
			
			try {
				onPoll(busy);
			}
			catch (std::exception& ex) {
				core_log_error("onPoll exception", ex.what());
			}

			if (!busy)
			{
				try {
					onIdel();
				}
				catch (std::exception& ex) {
					core_log_error("onIdel exception", ex.what());
				}
			}
		}
	}
	
}