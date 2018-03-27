#pragma once

#include <map>
#include <chrono>
#include <mutex>
#include "timeHelpers.h"


//******************************************************************************************
//
//	StampedSafeStorage
//	
//	Template for a thread-safe map which has a timepoint as the key
//
//******************************************************************************************


template <typename T>
class StampedSafeStorage {
	std::mutex mutex;
	std::map<std::chrono::system_clock::time_point, T> storage;

public:

	// Pushes a class instance in the template
	void push(const T&);

	// Pushes a time pair in storage
	void push(std::chrono::system_clock::time_point time, const T&);
	
	// Gets all data from storage
	std::map<std::chrono::system_clock::time_point, T> get();

	// Gets all data from storage that's later than a point tp
	std::map<std::chrono::system_clock::time_point, T> get(const std::chrono::system_clock::time_point & tp);

	// Deletes all data in storage
	void del();

	// Deletes datapoint with key tp
	void del(const std::chrono::system_clock::time_point & tp);

	// Deletes all data older than a point tp
	void del_until(const std::chrono::system_clock::time_point & tp);

	// Deletes first datapoint
	void del_first();

	// Gets size
	std::size_t size() { return storage.size(); };
};


template <typename T>
void StampedSafeStorage<T>::push(const T & value) {
	std::unique_lock<std::mutex> lock(mutex);
	storage.insert(std::make_pair(timeh::NowTime(), value));
}

template <typename T>
void StampedSafeStorage<T>::push(std::chrono::system_clock::time_point time, const T & value) {
	std::unique_lock<std::mutex> lock(mutex);
	storage.insert(std::make_pair(time, value));
}


template <typename T>
std::map<std::chrono::system_clock::time_point, T> StampedSafeStorage<T>::get() {
	std::unique_lock<std::mutex> lock(mutex);
	return std::map<std::chrono::system_clock::time_point, T>(storage);
}

template <typename T>
std::map<std::chrono::system_clock::time_point, T> StampedSafeStorage<T>::get(const std::chrono::system_clock::time_point & tp) {
	std::unique_lock<std::mutex> lock(mutex);
	return std::map<std::chrono::system_clock::time_point, T>(storage.upper_bound(tp), storage.end());
}

template <typename T>
void StampedSafeStorage<T>::del() {
	std::unique_lock<std::mutex> lock(mutex);
	storage.clear();
}

template <typename T>
void StampedSafeStorage<T>::del(const std::chrono::system_clock::time_point & tp) {
	std::unique_lock<std::mutex> lock(mutex);
	if (storage.find(tp) != storage.end())
	{
		storage.erase(tp);
	}
}

template <typename T>
void StampedSafeStorage<T>::del_until(const std::chrono::system_clock::time_point & tp) {
	std::unique_lock<std::mutex> lock(mutex);
	storage.erase(storage.begin(), storage.lower_bound(tp));
}

template <typename T>
void StampedSafeStorage<T>::del_first() {
	std::unique_lock<std::mutex> lock(mutex);
	storage.erase(storage.begin());
}


typedef std::map<std::chrono::system_clock::time_point, std::string> TextStorage;


//******************************************************************************************
//
//	time_point_mtx
//	
//	Template for a safe timepoint
//
//******************************************************************************************

class time_point_mtx
{
private:
	mutable std::mutex mtx;
	std::chrono::system_clock::time_point tp;

public:

	time_point_mtx() {}

	operator std::chrono::system_clock::time_point() {
		return load();												// return requested value
	}

	std::chrono::system_clock::time_point load() const {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		return tp;													// return requested value
	}

	operator std::chrono::system_clock::time_point() const {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		return tp;													// return requested value
	}

	time_point_mtx & operator=(const std::chrono::system_clock::time_point& rhs) {
		std::unique_lock<std::mutex> lock(mtx);						// lock
		tp = rhs;													// store
		return *this;												// return for operator chaining
	}

	friend bool operator<(const std::chrono::system_clock::time_point& l, const time_point_mtx& r)
	{
		std::unique_lock<std::mutex> lock(r.mtx);					// lock mutex
		return l < r.tp;											// return the comparison
	}
};

//******************************************************************************************
//
//	atomic_ts
//	
//	Template for an atomic parameter which timestamps a timepoint when set
//
//******************************************************************************************

template<typename T>
class atomic_ts
{
private:
	T value;
	mutable std::mutex mtx;
	time_point_mtx& tp;

public:
	atomic_ts() = delete;
	atomic_ts(time_point_mtx& t) : tp(t) {}

	operator T() const {
		return load();												// return requested value
	}
	T load() const {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		return value;												// return requested value
	}
	void store(const T& rhs) {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		value = rhs;												// save value
		tp = std::chrono::system_clock::now();						// Reset timepoint
	}
	void store_nostamp(const T& rhs) {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		value = rhs;												// save value
	}
	atomic_ts& operator=(const T& rhs) {
		store(rhs);													// Store
		return *this;												// return for operator chaining
	}
	atomic_ts& operator++() {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		++value;													// actual increment takes place here
		return *this;
	}
	bool operator==(const T& rhs) {
		std::unique_lock<std::mutex> lock(mtx);						// lock mutex
		return value == rhs;										// return equality
	}
	bool operator!=(const T& rhs) { return !(this->operator==(rhs)); }

	friend std::wostream& operator<<(std::wostream& os, const atomic_ts& t) {
		std::unique_lock<std::mutex> lock(t.mtx);					// lock mutex
		os << t.value;												// write value
		return os;													// return stream
	}
};
