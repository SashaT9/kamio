#include "lib.h"

std::filesystem::path kamio::xdg_data_home() {
	if (const char* xdg = std::getenv("XDG_DATA_HOME")) {
		return std::filesystem::path(xdg);
	}
	if (const char* home = std::getenv("HOME")) {
		return std::filesystem::path(home) / ".local" / "share";
	}
	throw std::runtime_error("xdg data home not set");
}

std::string kamio::format_duration(std::chrono::seconds duration, const bool days, const bool hours, const bool mins, const bool secs) {
	std::string res;
	if (days) {
		const auto d = std::chrono::duration_cast<std::chrono::days>(duration);
		duration -= d;
		res.append(std::to_string(d.count()) + "d ");
	}
	if (hours) {
		const auto h = std::chrono::duration_cast<std::chrono::hours>(duration);
		duration -= h;
		res.append(std::to_string(h.count()) + "h ");
	}
	if (mins) {
		const auto m = std::chrono::duration_cast<std::chrono::minutes>(duration);
		duration -= m;
		res.append(std::to_string(m.count()) + "m ");
	}
	if (secs) {
		const auto s = std::chrono::duration_cast<std::chrono::seconds>(duration);
		duration -= s;
		res.append(std::to_string(s.count()) + "s ");
	}
	return res;
}

kamio::TaskManager kamio::TaskManager::read(const nlohmann::json& j) {
	TaskManager res;
	for (auto it = j.begin(); it != j.end(); ++it) {
		long long secs = it.value().get<long long>();
		const auto tp = std::chrono::system_clock::time_point(std::chrono::seconds(secs));
		res.tasks[it.key()] = tp;
	}
	return res;
}

nlohmann::json kamio::TaskManager::write() {
	nlohmann::json j;
	for (const auto& [name, time] : tasks) {
		const auto secs = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
		j[name] = secs;
	}
	return j;
}

std::vector<std::string> kamio::TaskManager::get_names() {
	std::vector<std::string> names;
	for (const auto& [name, time] : tasks) {
		names.push_back(name);
	}
	return names;
}

void kamio::TaskManager::_do(const std::string& name) {
	tasks[name] = std::chrono::system_clock::now();
}

void kamio::TaskManager::_do(const std::vector<std::string>& names) {
	for (const auto& name : names) {
		_do(name);
	}
}

void kamio::TaskManager::_remove(const std::string& name) {
	if (tasks.contains(name)) {
		tasks.erase(name);
	}
}

void kamio::TaskManager::_remove(const std::vector<std::string>& names) {
	for (const auto& name : names) {
		_remove(name);
	}
}

template<typename Duration>
long long kamio::TaskManager::_view(const std::string& name) {
	if (!tasks.contains(name)) {
		return 0;
	}
	const auto now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<Duration>(now-tasks[name]).count();
}

template<typename Duration>
std::vector<std::pair<std::string, long long>> kamio::TaskManager::_view(const std::vector<std::string>& names) {
	std::vector<std::pair<std::string, long long>> res;
	for (const auto& name : names) {
		res.push_back(std::make_pair(name, _view<Duration>(name)));
	}
	return res;
}

template long long kamio::TaskManager::_view<std::chrono::seconds>(const std::string&);
template long long kamio::TaskManager::_view<std::chrono::minutes>(const std::string&);
template long long kamio::TaskManager::_view<std::chrono::hours>(const std::string&);
template long long kamio::TaskManager::_view<std::chrono::days>(const std::string&);

template std::vector<std::pair<std::string, long long>> kamio::TaskManager::_view<std::chrono::seconds>(const std::vector<std::string>&);
template std::vector<std::pair<std::string, long long>> kamio::TaskManager::_view<std::chrono::minutes>(const std::vector<std::string>&);
template std::vector<std::pair<std::string, long long>> kamio::TaskManager::_view<std::chrono::hours>(const std::vector<std::string>&);
template std::vector<std::pair<std::string, long long>> kamio::TaskManager::_view<std::chrono::days>(const std::vector<std::string>&);
