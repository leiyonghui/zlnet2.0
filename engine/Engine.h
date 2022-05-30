#pragma once
#include "Configs.h"
#include "core/TimerWheel.h"
#include "core/MsgQueue.h"
#include "Packet.h"

namespace engine
{
	class Engine : public timerwheel::TimerWheel
	{
	public:
		Engine();
		virtual~Engine();

		virtual void run();

		void bindMsgdispatcher(const std::function<void(Packet*)>& func);

		void bindMsgdispatcher(std::function<void(Packet*)>&& func);

	protected:
		virtual void onInit();

		virtual void onQuit();

		virtual void onPoll(bool& busy);

		virtual void onIdel();

		virtual void onTimer1000ms();

		virtual void onLoop();

	protected:
		bool _runing;
		int64 _lastClock1000ms;
		MsgQueue<Packet*>* _msgqueue;
	};
}
