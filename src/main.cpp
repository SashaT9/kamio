#include <CLI/CLI.hpp>
#include <iostream>
#include "lib.h"

const std::string APP_NAME = "kamio";
const std::string DATA_FILE_NAME = "kamio.json";

nlohmann::json load_data(const std::filesystem::path& path) {
	if (!std::filesystem::exists(path)) {
		return nlohmann::json::object();
	}
	std::ifstream in(path);
	return nlohmann::json::parse(in);
}

void save_data(const std::filesystem::path& path, const nlohmann::json& j) {
	std::ofstream out(path);
	out << j.dump();
}

int main(int argc, char **argv) {
	// CLI::App app{"Track how long ago you last did a task"};
	// app.require_subcommand(1);
	//
	// /* do task1, task2, ... */
	// auto _do = app.add_subcommand("do", "update tasks' dates to now. Creates tasks that didn't exist before");
	// std::vector<std::string> do_tasks;
	// _do->add_option("tasks", do_tasks, "list done tasks");
	//
	// /* view task */
	// auto _view = app.add_subcommand("view", "view all tasks, with how many minutes ago you last did them");
	// std::string view_task;
	// _view->add_option("view", view_task, "view task")->required();
	//
	// /* remove task1 task2 */
	// auto _remove = app.add_subcommand("remove", "remove specified tasks");
	// std::vector<std::string> removed_tasks;
	// _remove->add_option("tasks", removed_tasks, "list removed tasks");
	//
	// CLI11_PARSE(app, argc, argv);

	const std::filesystem::path data_dir = kamio::xdg_data_home() / APP_NAME;
	const std::filesystem::path path = data_dir / DATA_FILE_NAME;
	std::filesystem::create_directories(path.parent_path());

	auto j = load_data(path);
	save_data(path, j);
	auto tasks = kamio::TaskManager(kamio::TaskManager::read(j));
	return 0;
}