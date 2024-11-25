#pragma once
#include <thread>
#include <cmath>
#include <vector>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <functional>
#include <algorithm>

/*#ifdef _MSC_VER
#define WINDOWS 1
#include <Windows.h>
#endif*/

//Time taken to draw the last frame
//extern double frameTime;

template<class T>
using vector = std::vector<T>;


struct string : public std::string {
	using std::string::string;
	string() {}
	string(const std::string& s) : std::string(s) {}


	inline operator const char* () {
		return std::string::c_str();
	}
};

template<class A, class B>
using unordered_map = std::unordered_map<A, B>;

template<class T>
using function = std::function<T>;

#ifndef WINDOWS
using mutex = std::shared_mutex;
#else
struct mutex {
private:
	SRWLOCK mtx;
public:
	inline mutex() {
		InitializeSRWLock(&mtx);
	}

	inline void lock() {
		AcquireSRWLockExclusive(&mtx);
	}

	inline void unlock() {
		ReleaseSRWLockExclusive(&mtx);
	}
};
#endif

//Whether multiple threads are currently running (requiring mutexes to be used)
extern bool multiThreading;

//Optimized mutex implementation for mutexes that are only for synching sort threads
//  These mutexes only need to actually be locked/unlocked if more than one sorting thread is operating
//  If only one thread is running, it is allowed to gracefully bypass a matcing lock and unlock call

struct sortMutex : public mutex {
private:
	int grace = 0;
public:
	inline void lock() {
		if (multiThreading) {
			while (grace);
			mutex::lock();
		}
		else
			grace++;
	}
	inline void unlock() {
		if (grace)
			grace--;
		else
			mutex::unlock();
	}
};

using thread = std::thread;

struct timer {
	bool started = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> last;
	double mem = 0.; //Last interval duration

	inline void start() {
		started = true;
		last = std::chrono::high_resolution_clock::now();
	}

	//Elapsed time in milliseconds (nano frequency)
	inline double elapsed() {
		auto now = std::chrono::high_resolution_clock::now();
		double res = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count() / 1000000.0;
		return res;
	}

	inline double elapsedSeconds() {
		return elapsed() / 1000.0;
	}

	//Returns elapsed time and starts the timer from 0
	inline double interval() {
		auto now = std::chrono::high_resolution_clock::now();
		if (!started) {
			started = true;
			last = now;
			return 0.0;
		}
		double res = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now - last).count() / 1000000.0;
		last = now;
		mem = res;
		return res;
	}

	inline double intervalSeconds() {
		return interval() / 1000.0;
	}
};