#pragma once
#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

namespace kamio {
	// returns $XDG_DATA_HOME or $HOME/.local/share
	std::filesystem::path xdg_data_home();

	std::string format_duration(const std::chrono::seconds duration, bool days, bool hours, bool mins, bool secs);

	class TaskManager {
		std::unordered_map<std::string, std::chrono::system_clock::time_point> tasks;

		void _do(const std::string& name);

		template<typename Duration>
		long long _view(const std::string& name);

		void _remove(const std::string& name);
	public:
		static TaskManager read(const nlohmann::json& j);
		nlohmann::json write();
		std::vector<std::string> get_names();

		void _do(const std::vector<std::string>& names);

		template<typename Duration>
		std::vector<std::pair<std::string, long long>> _view(const std::vector<std::string>& names);

		void _remove(const std::vector<std::string>& names);
	};
}
