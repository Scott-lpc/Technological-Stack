// LiuXingProj.cpp: ����Ӧ�ó������ڵ㡣
//
#include <vector>
#include "LxCodeUI.h"
#include "LxConsole.h"
#include "StopWatch.h"
#include "SolidCubeCase.h"
#include "TwoCubeCase.h"
#include "UserDefineSourceCase.h"
#include "CavityFlow.h"
#include "LaminarNaturalConvection.h"
void Run()
{
	using namespace std;
	using namespace LxCFD;
	string folder = "D:/LxCases/";
	vector<std::unique_ptr<ExampleBase>> exampleList;
	exampleList.push_back(std::unique_ptr<ExampleBase>(new SolidCubeCase(exampleList.size(), folder)));
	exampleList.push_back(std::unique_ptr<ExampleBase>(new TwoCubeCase(exampleList.size(), folder)));
	exampleList.push_back(std::unique_ptr<ExampleBase>(new UserDefineSourceCase(exampleList.size(), folder)));
	exampleList.push_back(std::unique_ptr<ExampleBase>(new CavityFlow(exampleList.size(), folder)));
	exampleList.push_back(std::unique_ptr<ExampleBase>(new LaminarNaturalConvection(exampleList.size(), folder)));
	cout << "��������ѡ���Ӧ��������ǰ�������0~" << exampleList.size() - 1
		 << "���ļ�Ŀ¼" << folder << "��" << endl;
	for (auto &exm : exampleList)
	{
		cout << exm->ID << "    " << exm->Description << endl;
	}
	size_t select;
	cin >> select;
	cin.clear();
	cin.ignore();
	if (select < exampleList.size())
	{
		exampleList.at(select)->RunCase();
	}
	else
	{
		stringstream ss;
		ss << "������Ч���֣���ǰ�������0~" << exampleList.size();
		LxConsole::Error(ss.str());
	}
	cout << "Press any key to exit" << endl;
	cin.get();
	ComputeState::ReportInterrupt_ByUser();
	for (;;)
	{
		if (!ComputeState::GetIsRunning())
		{
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void test()
{
	using namespace std;
	using namespace LxCFD;
	LinkArray<double>::Deep deep;
	deep.Initialize(3, 5);
	LinkArray<double>::List la;
	LinkArray<double>::List lb;
	LinkArray<double>::List lc;
	deep.CreateAdd(la, (1.1)); //	deep.AddTo(la, 1.1);
	deep.CreateAdd(la, 1.2);
	deep.CreateAdd(lb, 2.1);
	deep.CreateAdd(lb, 2.2);
	deep.CreateAdd(la, 1.3);
	deep.CreateAdd(la, 1.4);
	lc.AppendMoveRange(la);
	lc.AppendMoveRange(lb);
	cout << deep.ToString();
	cout << la.ToString();
	cout << lb.ToString();
	cout << lc.ToString();
	LinkArray<double>::List ld;
	ld.AppendMoveRange(la);
	cout << ld.ToString();
	cout << "Press any key to exit" << endl;
	cin.get();
}
int main()
{
	using namespace std;
	using namespace LxCFD;
	Run();
	//test();
	return 0;
}
