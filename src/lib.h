#pragma once
#include <chrono>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace kamio {
	// returns $XDG_DATA_HOME or $HOME/.local/share
	std::filesystem::path xdg_data_home();

	class TaskManager {
		std::unordered_map<std::string, std::chrono::time_point<std::chrono::system_clock>> tasks;
	public:
		void _do(const std::string& name);
		void _do(const std::vector<std::string>& names);

		void _remove(const std::string& name);
		void _remove(const std::vector<std::string>& names);

		long long _view(const std::string& name);
	};
}
