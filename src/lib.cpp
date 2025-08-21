#include "lib.h"

std::filesystem::path kamio::xdg_data_home() {
	if (const char* xdg = std::getenv("XDG_DATA_HOME")) {
		return std::filesystem::path(xdg);
	} else if (const char* home = std::getenv("HOME")) {
		return std::filesystem::path(home) / ".local" / "share";
	}
	throw std::runtime_error("xdg data home not set");
}

void kamio::TaskManager::_do(const std::string &name) {
	tasks[name] = std::chrono::system_clock::now();
}

void kamio::TaskManager::_do(const std::vector<std::string> &names) {
	for (const auto& name : names) {
		_do(name);
	}
}

void kamio::TaskManager::_remove(const std::string &name) {
	if (tasks.count(name)) {
		tasks.erase(name);
	}
}

void kamio::TaskManager::_remove(const std::vector<std::string> &names) {
	for (const auto& name : names) {
		_remove(name);
	}
}

long long kamio::TaskManager::_view(const std::string &name) {
	if (!tasks.count(name)) {
		return 0;
	}
	const auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::seconds>(now-tasks[name]).count();
}

