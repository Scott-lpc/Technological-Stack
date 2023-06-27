#include "LinearSolverBase.h"

namespace LxCFD
{

    void LinearSolverBase::prolong()
    {
        MultiThreads::RunCalTasks(
            [&, this](int taskIndex)
            {
                for (auto &z : AMGsInZone)
                {
                    z->Prolong(taskIndex);
                }
            });
        --AMGBase::SolvingLevel;
    }
    void LinearSolverBase::setIsUpdateAFalse()
    {
        for (auto &z : AMGsInZone)
        {
            z->isNeededUpdateA = false;
        } //����һ�κ󣬺��治��Ҫ������
        for (auto &c : AMGsInContact)
        {
            c->isNeededUpdateA = false;
        }
    }
    void LinearSolverBase::iniVStartLevel()
    {
        AMG_V_StartLevel = AMGBase::SolvingLevel;
    }
    void LinearSolverBase::restrict()
    {
        ++AMGBase::SolvingLevel;
        MultiThreads::RunCalTasks(
            [&, this](int taskIndex)
            {
                for (auto &z : AMGsInZone)
                {
                    z->Restrict(taskIndex);
                }
                for (auto &c : AMGsInContact)
                {
                    c->Restrict(taskIndex);
                }
            });
    }
}