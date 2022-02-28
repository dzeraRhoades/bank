#include "Bank.h"
#include<random>
#include<string>
#include"BinomialHeap.h"
#include"logBuilder.h"
#include<Windows.h>
//#include<iostream>
int apps_count = 0;
int Bank::Application::app_nomber = 0;
int Bank::Department::department_id = 0;

Bank::~Bank()
{
	for (auto i : departments)
		delete i;
}

void Bank::add_dep(int emp_num)
{
	Department* dep = new Department(emp_num);
	departments.push_back(dep);
}
//void Bank::initialisation()
//{
//
//}
void Bank::bank_configuration(int average, int dev, int lmax, double l)
{
	t = average;
	td = dev;
	this->l = l;
	this->lmax = lmax;
}
void Bank::work_day()
{
	int cur_time = 0;

	// запись в файлы о начале рабочего дня
	for (int i = 0; i < departments.size(); i++)
		departments[i]->logger->log(INFORMATION, "opened", cur_time);
	while (cur_time != 780) // 13 hours work day
	{
		
		staff_come_to_work_and_go_home(cur_time);
		application_generator(cur_time);
		applications_distribution(cur_time);
		main_action(cur_time);

		cur_time++;
	}
	// запись в файл о конце рабочего дня
	for (auto dep : departments)
	{
		while (1)
		{
			try
			{
				Application* app = dep->unprocessed_apps->delMin();
				dep->logger->log(INFORMATION, "app " + std::to_string(app->app_id) + " wasn't finished", 11 * 60); // unfinished applications
				delete app;
			}
			catch (const std::exception&)
			{
				break;
			}
		}
		dep->logger->log(INFORMATION, "closed", cur_time);
	}
	std::cout << "<<<<<<<<<<<<<<<<<<" << apps_count << std::endl;
}
void Bank::staff_come_to_work_and_go_home(int cur_time)
{
	if (cur_time < 240) // потенциальное время прихода на работу
	{
		for (int i = 0; i < departments.size(); i++)
		{
			for (int j = 0; j < departments[i]->all_staff.size(); j++)
			{

				if (departments[i]->all_staff[j]->work_coming_time == cur_time)
				{
					Employee* worker = departments[i]->all_staff[j];
					worker->is_at_work = true; // говорим что работник пришел
					departments[i]->free_staff.push(worker); // закидываем его в очередь на заявку
					departments[i]->cur_staff++;
					departments[i]->logger->log(INFORMATION, "worker " + std::to_string(worker->worker_id) + " has come to work", cur_time); // worker come to work
				}
			}
		}
	}
	else if (cur_time >= 540) // потенциальное время ухода с работы
	{
		for (int i = 0; i < departments.size(); i++)
		{
			for (int j = 0; j < departments[i]->all_staff.size(); j++)
			{

				if (departments[i]->all_staff[j]->going_home_time == cur_time) // если пора уходить и он не выполняет заявку
				{
					Employee* worker = departments[i]->all_staff[j];
					if (worker->application_time != 0)
					{
						departments[i]->unprocessed_apps->insert(worker->cur_application_task->time, worker->cur_application_task);
						worker->cur_application_task = nullptr;
						worker->application_time = 0;
					}
					worker->is_at_work = false; // говорим что работник ушел
					departments[i]->cur_staff--;
					departments[i]->logger->log(INFORMATION, "worker " + std::to_string(worker->worker_id) + " has left work", cur_time);
				}
			}
		}
	}
}
void Bank::application_generator(int cur_time)
{
	//srand(time(NULL));
	bool create_application; 
	Department* free_dep;
	while (create_application = rand() % 2)
	{
		apps_count++;
		Application* app = new Application;
		app->urgency = rand() % 10;
		app->time = cur_time;
		app->dep_id = rand() % departments.size();
		app->difficulty = rand() % 6;
		app->info = "this fckn application is to be done in time!!! app no: " + std::to_string(app->app_id);

		// тут может произойти перегрузка отделения
		//overload_chance = rand() % 10;
		
		if (departments[app->dep_id]->overload_time) // если данное отделение перегружено кидать заявку наименее загруженному
		{
			free_dep = find_most_free_dep();
			free_dep->unprocessed_apps->insert(app->urgency, app);
			free_dep->unprocessed_apps_number++;
			free_dep->logger->log(INFORMATION, "application received", cur_time);
			continue;
		}
		else
		{
			//Sleep(100);
			departments[app->dep_id]->unprocessed_apps->insert(app->urgency, app);
			departments[app->dep_id]->unprocessed_apps_number++;
			departments[app->dep_id]->logger->log(INFORMATION, "application " + std::to_string(app->app_id)+" received", cur_time);

			std::cout << "application " + std::to_string(app->app_id) + " received " << app->dep_id << std::endl;
		}
		if (departments[app->dep_id]->unprocessed_apps_number > l && overloads_times <= lmax) // если необработанных заявок в отделении больше допустимого значение l
		{
			overloads_times++;
			free_dep = find_most_free_dep(); // searching for the most free department, to send the applications from the overloaded one
			departments[app->dep_id]->overload(free_dep, cur_time);
		}
	}
	
}

void Bank::main_action(int cur_time)
{
	Employee* worker;
	Department* dep;
	for (int i = 0; i < departments.size(); i++)
	{
		dep = departments[i];
		if (dep->overload_time) // если перегружено
		{
			dep->overload_time--;
			if (dep->overload_time == 0) // отделение только что вышла из времени перегрузки
				dep->logger->log(INFORMATION, "\t\tdep returned to normal work", cur_time);
			else
				continue;
		}
		for (int j = 0; j < dep->all_staff.size(); j++)
		{
			worker = dep->all_staff[j];
			if (worker->is_at_work) // если он на работе
			{
				if (worker->rest_time == 0) // если у него не отдых
				{
					
					if (worker->application_time != 0)
					{
						worker->application_time--;
						if (worker->application_time == 0) // если он только что закончил, можно отправить на отдых
						{
							apps_count--;
							//логируем завершение и удаляем заявку
							dep->logger->log(INFORMATION, "worker " + std::to_string(worker->worker_id) + " finished app " + std::to_string(worker->cur_application_task->app_id), cur_time);
							delete worker->cur_application_task;
							worker->cur_application_task = nullptr;
							// отправляем на отдых
							if (worker->go_rest(cur_time))
								dep->logger->log(INFORMATION, dep->worker_info(worker) + "goes to rest", cur_time); // logging worker goin to rest
							else
								dep->free_staff.push(worker);
							/*delete worker->cur_application_task;
							worker->cur_application_task = nullptr;*/
						}
							
					}
				}
				else
				{
					worker->rest_time--;
					if (worker->rest_time == 0)
					{
						dep->free_staff.push(worker);
						dep->logger->log(INFORMATION, "worker " + std::to_string(worker->worker_id) + " stoped resting", cur_time);
					}
						
				}
			}
		}
	}
}
void Bank::applications_distribution(int cur_time)
{
	Department* dep;
	Employee* worker;
	Application* app;
	int plus_or_minus;
	//srand(time(NULL));
	for (int i = 0; i < departments.size(); i++)
	{
		dep = departments[i];
		if (dep->overload_time)
			continue;
		while (!dep->free_staff.empty() && dep->unprocessed_apps_number != 0)
		{
			worker = dep->free_staff.front();
			while (!worker->is_at_work)
			{
				dep->free_staff.pop();
				if (!dep->free_staff.empty())
					worker = dep->free_staff.front();
				else
				{
					worker = nullptr;
					break;
				}
			}
			if (worker == nullptr)
				break;
			app = dep->unprocessed_apps->delMin();
			worker->cur_application_task = app;
			dep->free_staff.pop();
			dep->unprocessed_apps_number--;
			plus_or_minus = rand() % 2;
			worker->application_time = t + (plus_or_minus == 0 ? -td : td) + app->difficulty * 2;
			dep->logger->log(INFORMATION, "worker " + std::to_string(worker->worker_id) + " started app " + std::to_string(app->app_id), cur_time);
			
		}
	}
}
Bank::Department* Bank::find_most_free_dep()
{
	Department* free = departments[0];
	Department* cur_dep;
	for (int i = 0; i < departments.size(); i++)
	{
		cur_dep = departments[i];
		if(cur_dep->cur_staff != 0 && free->cur_staff != 0)
			if ((cur_dep->unprocessed_apps_number / cur_dep->cur_staff) < (free->unprocessed_apps_number / free->cur_staff))
			{
				free = cur_dep;
			}
	}

	return free;
}
void Bank::start()
{
	work_day();
}
//int Bank::Department::count = 0;

Bank::Department::Department(int k)
{
	department_id++;
	cmp = new CmpForInt;
	logBuilder lg_bld;
	logger = lg_bld.buildLog("logfile"+std::to_string(department_id)+".txt");
	unprocessed_apps = new BinomialHeap<int, Application*>(cmp);
	all_staff_num = k;
	for (int i = 0; i < k; i++)
	{
		all_staff.push_back(new Employee(i));
	}
}

Bank::Department::~Department()
{
	delete logger;
	
	while (1)
	{
		try
		{
			Application* app = unprocessed_apps->delMin();
			delete app;
		}
		catch (const std::exception&)
		{
			break;
		}
	}
	delete unprocessed_apps;
	delete cmp;
	for (auto item : all_staff)
		delete item;
}

std::string Bank::Department::time_info(int cur_time)
{
	return (std::to_string(8 + cur_time / 60) + ":" + std::to_string(cur_time % 60));
}

void Bank::Department::overload(Department* free_dep, int cur_time)
{
	for (auto i : all_staff)
	{
		if (i->cur_application_task != nullptr)
		{
			unprocessed_apps->insert(i->cur_application_task->urgency, i->cur_application_task);
			i->cur_application_task = nullptr;
			i->application_time = 0;
		}
	}
	free_dep->unprocessed_apps->merge(unprocessed_apps);
	unprocessed_apps_number = 0;
	overload_time = 60 + (rand() % 120);
	logger->log(INFORMATION, "\t\tdepartment is overloaded. Overload time: " + std::to_string(overload_time), cur_time);
}

std::string Bank::Department::worker_info(Employee* worker)
{
	return ("Employee [" + std::to_string(worker->worker_id) + "]");
}

std::string Bank::Department::app_info(Application* app)
{
	std::string info = "Application [" + std::to_string(app->app_id) + "] [urgency=" + std::to_string(app->urgency) + "] [receiving time=" + std::to_string(app->time) + "]";
	return info;
}

//void Bank::Department::WLogging(LOG_TYPE log_tp, int cur_time, Employee* worker)
//{
//	std::string _time = std::to_string(8 + cur_time / 60) + ":" + std::to_string(cur_time % 60);
//	std::string info;
//	switch (log_tp)
//	{
//	/*case Bank::Department::WORK_START:
//		info = "The department number starts working";
//		break;*/
//	case Bank::Department::W_START_APP:
//		info = "The " + std::to_string(worker->worker_id)+ " employee from the department has started the application nomber: " + std::to_string(worker->cur_application_task->app_id);
//		break;
//	case Bank::Department::W_FINISHED_APP:
//		info = "The department has finished the application nomber: " + std::to_string(worker->cur_application_task->app_id);
//		break;
//	/*case Bank::Department::APP_ADD:
//		info = "The department has received the application nomber: " + std::to_string(app->app_id);
//		break;*/
//	/*case Bank::Department::OVERLOAD:
//		info = "The department is overloaded";
//		break;*/
//	/*case Bank::Department::TAKE_APPS:
//		info = "The department has taken the applications from overloaded department";
//		break;*/
//	/*case Bank::Department::DEP_BACK_WORK:
//		info = "The department is back to work";
//		break;*/
//	case Bank::Department::W_COME:
//		info = "The " + std::to_string(worker->worker_id) + " employee has come to work";
//		break;
//	case Bank::Department::W_GO:
//		info = "The " + std::to_string(worker->worker_id) + " employee goes home";
//		break;
//	case Bank::Department::W_REST:
//		info = "The " + std::to_string(worker->worker_id) + " employee starts rest";
//		break;
//	case Bank::Department::W_STOP_REST:
//		info = "The " + std::to_string(worker->worker_id) + " employee stop rest and back to work";
//		break;
//	/*case Bank::Department::WORK_OVER:
//		info = "The department stop working";
//		break;*/
//	default:
//		break;
//	}
//	logger->log(INFORMATION, info, _time);
//}

//void Bank::Department::overload(int cur_time)
//{
//	srand(time(NULL));
//	overload_time = 60 + (rand() % 120);
//	for (int i = 0; i < all_staff.size(); i++)
//	{
//		if (all_staff[i]->application_time)
//			all_staff[i]->application_time = 0;
//	}
//}

Bank::Employee::Employee(int id)
{
	worker_id = id;
	srand(worker_id + time(nullptr));
	work_coming_time = rand() % 240;
	going_home_time = 540 + (rand() % 240);
}

bool Bank::Employee::go_rest(int cur_time)
{
	if (have_already_rested)
		return false;
	srand(time(NULL));
	int chance_to_go_rest = rand() % 100;
	if (cur_time < 240 + work_coming_time)
	{
		if (chance_to_go_rest < 15)
		{
			rest_time = 60;
			have_already_rested = true;
			return true;
		}
	}
	else
	{
		if (chance_to_go_rest < 75)
		{
			rest_time = 60;
			have_already_rested = true;
			return true;
		}
	}
	return false;
}


