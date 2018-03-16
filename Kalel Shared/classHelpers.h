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

	// Deletes all data older than a point tp
	void del(const std::chrono::system_clock::time_point & tp);

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
//	StampedSafeStorage
//	
//	Template for a thread-safe class attribute which updates a timestamp when it is changed
//
//******************************************************************************************

template<typename T>
class Attribute
{
public:
	Attribute(std::mutex & mtx, std::chrono::system_clock::time_point & tc);
protected:
	std::mutex & mtx;
	std::chrono::system_clock::time_point & tc;
	T val;
public:
	T get() {
		std::lock_guard<std::mutex> lock(mtx);
		return val;
	}
	T& get() const {
		std::lock_guard<std::mutex> lock(mtx);
		return &val;
	}
	void set(T v) {
		std::lock_guard<std::mutex> lock(mtx);
		tc = timeh::NowTime();
		val = v;
	}
	T & operator=(const T &rhs);
	T & operator==(const T &rhs);
	T & operator!=(const T &rhs);
	T operator++();
};

template<typename T>
inline Attribute<T>::Attribute(std::mutex & mtx, std::chrono::system_clock::time_point & tc)
	: mtx(mtx), tc(tc) {}

template<typename T>
inline T & Attribute<T>::operator=(const T & rhs)
{
	if (this != &rhs) {
		this->set(rhs);
	}
	return *this;
}

template<typename T>
inline T & Attribute<T>::operator==(const T & rhs)
{
	return this->get() == rhs;
}

template<typename T>
inline T & Attribute<T>::operator!=(const T & rhs)
{
	return this->get() != rhs;
}

template<typename T>
inline T Attribute<T>::operator++()
{
	std::lock_guard<std::mutex> lock(mtx);
	this->val = val + 1;
}
