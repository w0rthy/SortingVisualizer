#pragma once
#include "../common.h"

struct TokenSystem {
	//Used when modifying armed
	mutex mtx;
	//Whether there is a token alive in the system
	bool armed = false;
};

struct TokenState {
	//System this state belongs to
	TokenSystem* system;
	//Only used when modifying action
	mutex mtx;
	//Whether something intends to use this state
	bool intent = false;
	//Whether this token currently allows actions to be performed
	bool action = false;
	//The next token in the system
	TokenState* nxt = nullptr;
	
	//Funciton to determine whether or not this TokenState should be given a turn when it has intent (true) or skipped (false)
	function<bool()> intentFunc;
	//Function to run when a token is received (action begins)
	function<void()> onActionBegin;
	//Function to run when a token is forfeited (action ends)
	function<void()> onActionEnd;

	TokenState(TokenSystem* system) {
		this->system = system;
	}

	inline operator bool() {
		return action;
	}

	//Attempt to pass through the gate
	void operator()() {
		while (!action) {
			if (!system->armed) {
				system->mtx.lock();
				if (!system->armed) {
					if (onActionBegin)
						onActionBegin();
					action = true;
					system->armed = true;
					system->mtx.unlock();
					return;
				}
				system->mtx.unlock();
				if (action)
					return;
			}
			intent = true;
			//Might not want to sleep here?? Busy wait instead for much better responsiveness
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}

	//Finish the current action and progress the token 
	void operator--(int) {
		if (action) {
			mtx.lock();
			if (action) {
				if (onActionEnd)
					onActionEnd();
				action = false;
				intent = false;
				auto tmp = nxt;
				bool success = false;
				while (tmp != nullptr && tmp != this) {
					if (tmp->intent && (!tmp->intentFunc || tmp->intentFunc())) {
						if (tmp->onActionBegin)
							tmp->onActionBegin();
						tmp->action = true;
						success = true;
						break;
					}
					tmp = tmp->nxt;
				}
				if (!success)
					system->armed = false;
			}
			mtx.unlock();
		}
	}

	//Finish the current action and progress the token but keep intent
	void operator++(int) {
		if (action) {
			mtx.lock();
			if (action) {
				if (onActionEnd)
					onActionEnd();
				action = false;
				auto tmp = nxt;
				bool success = false;
				while (tmp != nullptr && tmp != this) {
					if (tmp->intent && (!tmp->intentFunc || tmp->intentFunc())) {
						if (tmp->onActionBegin)
							tmp->onActionBegin();
						tmp->action = true;
						intent = true;
						success = true;
						break;
					}
					tmp = tmp->nxt;
				}
				if (!success)
					action = true;
			}
			mtx.unlock();
		}
	}
};