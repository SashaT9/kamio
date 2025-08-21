#include <CLI/CLI.hpp>
#include <iostream>
#include "lib.h"

const std::string APP_NAME = "kamio";
const std::string DATA_FILE_NAME = "kamio.json";

std::unique_ptr<CLI::App> args(kamio::TaskManager& tasks) {
	auto app = std::make_unique<CLI::App>("Track how long ago you last did a task");
	app->require_subcommand(1);

	/* do task1, task2, ... */
	const auto _do = app->add_subcommand("do", "update tasks' dates to now. Creates tasks that didn't exist before");
	const auto do_tasks = std::make_shared<std::vector<std::string>>();
	_do->add_option("tasks", *do_tasks, "list done tasks");
	_do->callback([&, do_tasks] { tasks._do(*do_tasks); });

	/* view task */
	const auto _view = app->add_subcommand("view", "view all tasks, with how many minutes ago you last did them");
	const auto view_task = std::make_shared<std::string>();
	_view->add_option("view", *view_task, "view task")->required();
	_view->callback([&, view_task] { tasks._view(*view_task); });

	/* remove task1 task2 */
	const auto _remove = app->add_subcommand("remove", "remove specified tasks");
	const auto removed_tasks = std::make_shared<std::vector<std::string>>();
	_remove->add_option("tasks", *removed_tasks, "list removed tasks");
	_remove->callback([&, removed_tasks] { tasks._remove(*removed_tasks); });

	return app;
}

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
	const std::filesystem::path data_dir = kamio::xdg_data_home() / APP_NAME;
	const std::filesystem::path path = data_dir / DATA_FILE_NAME;
	std::filesystem::create_directories(path.parent_path());

	auto j = load_data(path);
	auto tasks = kamio::TaskManager(kamio::TaskManager::read(j));

	const auto app = args(tasks);
	CLI11_PARSE(*app, argc, argv);
	return 0;
}