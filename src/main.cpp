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
	auto do_tasks = std::make_shared<std::vector<std::string>>();
	_do->add_option("tasks", *do_tasks, "list done tasks");
	_do->callback([&, do_tasks] { tasks._do(*do_tasks); });

	/* view tasks task1, task2, ... */
	/* default all */
	const auto _view = app->add_subcommand("view", "view all tasks, with how many minutes ago you last did them");
	auto days = std::make_shared<bool>(false);
	auto hours = std::make_shared<bool>(false);
	auto minutes = std::make_shared<bool>(false);
	auto seconds = std::make_shared<bool>(false);
	auto json = std::make_shared<bool>(false);
	_view->add_flag("-D", *days, "display days");
	_view->add_flag("-H", *hours, "display hours");
	_view->add_flag("-M", *minutes, "display minutes");
	_view->add_flag("-S", *seconds, "display seconds");
	_view->add_flag("-j, --json", *json, "display in json");
	auto view_tasks = std::make_shared<std::vector<std::string>>(tasks.get_names());
	_view->add_option("view", *view_tasks, "view tasks");
	_view->callback([&, view_tasks, days, hours, minutes, seconds, json] {
		if (!*days && !*hours && !*minutes && !*seconds) {
			*days = true;
			*hours = true;
			*minutes = true;
			*seconds = false;
		}
		if (*json) {
			nlohmann::json j;
			for (const auto& [name, time] : tasks._view<std::chrono::seconds>(*view_tasks)) {
				auto dur = kamio::Duration(std::chrono::seconds(time), *days, *hours, *minutes, *seconds);
				j[name] = dur.to_json(*days, *hours, *minutes, *seconds);
			}
			std::cout << j.dump() << std::endl;
		} else {
			for (const auto& [name, time] : tasks._view<std::chrono::seconds>(*view_tasks)) {
				auto dur = kamio::Duration(std::chrono::seconds(time), *days, *hours, *minutes, *seconds);
				std::cout << std::left << std::setw(10) << name << std::right << std::setw(5) << dur.to_string(*days, *hours, *minutes, *seconds) << std::endl;
			}
		}
 	});

	/* remove task1 task2 */
	const auto _remove = app->add_subcommand("remove", "remove specified tasks");
	auto removed_tasks = std::make_shared<std::vector<std::string>>();
	_remove->add_option("tasks", *removed_tasks, "list removed tasks");
	_remove->callback([&, removed_tasks] { tasks._remove(*removed_tasks); });

	return app;
}

nlohmann::json load_data(const std::filesystem::path& path) {
	if (!std::filesystem::exists(path)) {
		return nlohmann::json();
	}
	std::ifstream in(path);
	return nlohmann::json::parse(in);
}

void save_data(const std::filesystem::path& path, const nlohmann::json& j) {
	std::ofstream out(path);
	out << j.dump(4);
}

int main(int argc, char **argv) {
	const std::filesystem::path data_dir = kamio::xdg_data_home() / APP_NAME;
	const std::filesystem::path path = data_dir / DATA_FILE_NAME;
	std::filesystem::create_directories(path.parent_path());

	auto j = load_data(path);
	auto tasks = kamio::TaskManager::read(j);

	const auto app = args(tasks);
	CLI11_PARSE(*app, argc, argv);

	j = tasks.write();
	save_data(path, j);

	return 0;
}