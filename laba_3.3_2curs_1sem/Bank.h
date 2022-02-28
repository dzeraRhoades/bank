#pragma once
#include<iostream>
#include<map>
#include<stack>
#include<queue>
#include<vector>
#include"BinomialHeap.h"
#include"logBuilder.h"


class Bank
{
	friend class BankBuilder;
	struct Application;

	struct Employee
	{
		Employee(int id);
		//std::string name;
		bool is_at_work = false;
		int rest_time = 0;
		Application* cur_application_task = nullptr;
		int application_time = 0;
		int work_coming_time;
		int going_home_time;
		bool go_rest(int cur_time);
		//void finish_application(int cur_time);
		bool have_already_rested = false;
		int worker_id;
	};
	struct Application
	{
		Application() { app_nomber++; app_id = app_nomber; }
		static int app_nomber;
		int urgency;
		int time;
		int dep_id;
		int difficulty;
		int app_id;
		std::string info;
	};
	struct Department
	{
		enum LOG_TYPE { W_START_APP, W_FINISHED_APP, W_COME, W_GO, W_REST, W_STOP_REST};
		Heap<int, Application*>* unprocessed_apps;
		std::vector<Employee*> all_staff;
		std::queue<Employee*> free_staff;
		//static int count;
		Department(int k);
		~Department();
		std::string time_info(int cur_time);
		void overload(Department* free_dep, int cur_time);
		std::string worker_info(Employee* worker);
		std::string app_info(Application* app);
		//void WLogging(LOG_TYPE log_tp, int cur_time, Employee* worker); // функция помогает логировать все произошедвшие с отделением события
		//void overload(int cur_time);
		int overload_time = 0;
		int cur_staff = 0;
		int all_staff_num;
		int unprocessed_apps_number = 0;
		Logger* logger;
		Cmp<int>* cmp;
		static int department_id;
	};
	Bank(){}
	
private:
	//int dep_num;
	std::vector<Department*> departments; // номер отделения - его индекс в массиве
	int t = 0;
	int td = 0;
	int l = 0;
	int lmax = 0;
	int overloads_times = 0;
	//int unloaded_dep_id = -1; // индекс наименее загруженного отделения
private:
	void add_dep(int emp_num);
	void bank_configuration(int average, int dev, int lmax, double l);
	//void initialisation();
	void work_day();
	void staff_come_to_work_and_go_home(int cur_time);
	void application_generator(int cur_time);
	
	void main_action(int cur_time); // в этой функции будут происходить основыне действия программы
	void applications_distribution(int cur_time);
	Department* find_most_free_dep(); // функция ищет наименее загруженный отдел
public:
	void start();
	~Bank();
};

