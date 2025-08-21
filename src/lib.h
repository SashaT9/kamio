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

	class TaskManager {
		std::unordered_map<std::string, std::chrono::system_clock::time_point> tasks;
	public:
		static std::unordered_map<std::string, std::chrono::system_clock::time_point> read(const nlohmann::json& j);
		static nlohmann::json write(const std::unordered_map<std::string, std::chrono::system_clock::time_point>& tasks);

		void _do(const std::string& name);
		void _do(const std::vector<std::string>& names);

		void _remove(const std::string& name);
		void _remove(const std::vector<std::string>& names);

		long long _view(const std::string& name);
	};
}
