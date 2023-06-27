#pragma once
#include "IReset.h"
#include "ResidualMonitor.h"
#include "UserMonitorBase.h"
#include <vector>
namespace LxCFD
{
    class MonitorCenter : public IReset
    {
    public:
        MonitorCenter();
        ResidualMonitor Continuity = ResidualMonitor{"������"};
        ResidualMonitor Vel_U = ResidualMonitor{"����U"};
        ResidualMonitor Vel_V = ResidualMonitor{"����V"};
        ResidualMonitor Vel_W = ResidualMonitor{"����W"};
        ResidualMonitor Temperature = ResidualMonitor{"�¶�"};
        ResidualMonitor Turb_k = ResidualMonitor{"�Ķ���"};
        ResidualMonitor Turb_omega = ResidualMonitor{"�Ⱥ�ɢ��"};
        std::vector<ResidualMonitor *> ResidualMonitors;
        std::vector<std::unique_ptr<UserMonitorBase>> UserMonitors;
        virtual void Reset(ResetTypes clearType) override;
        void Monitor_Steady();
        void Monitor_Transient();
    };
}