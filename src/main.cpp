#include <CLI/CLI.hpp>
#include <iostream>
#include "lib.h"

int main(int argc, char **argv) {
	CLI::App app{"Track how long ago you last did a task"};
	app.require_subcommand(1);

	/* do task1, task2, ... */
	auto _do = app.add_subcommand("do", "update tasks' dates to now. Creates tasks that didn't exist before");
	std::vector<std::string> do_tasks;
	_do->add_option("tasks", do_tasks, "list done tasks");

	/* view */
	auto _view = app.add_subcommand("view", "view all tasks, with how many minutes ago you last did them");
	std::string view_task;
	_view->add_option("view", view_task, "view task");

	/* remove task1 task2 */
	auto _remove = app.add_subcommand("remove", "remove specified tasks");
	std::vector<std::string> removed_tasks;
	_remove->add_option("tasks", removed_tasks, "list removed tasks");

	CLI11_PARSE(app, argc, argv);
	return 0;
}