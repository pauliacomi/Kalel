#pragma once

#include <map>
#include <chrono>
#include <mutex>

typedef std::map<std::chrono::system_clock::time_point, std::string> TextStorage;

template <typename T>
class StampedSafeStorage {
	std::mutex mutex;
	std::map<std::chrono::system_clock::time_point, T> storage;

public:

	// Pushes a time pair in storage
	void push(std::chrono::system_clock::time_point time, T);
	
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
void StampedSafeStorage<T>::push(std::chrono::system_clock::time_point time, T value) {
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