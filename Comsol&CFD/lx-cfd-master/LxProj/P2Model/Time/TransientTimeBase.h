#pragma once
#include <memory>
#include "TransientDataBase.h"

namespace LxCFD
{
    class TransientTimeBase
    {
    public:
        double ResultTime=0; //������ʾ��ʽ��Result��ʱ������ɢ���̵���һ����������ʽ��һ���ģ�ResultTime��Ϊtʱ�䣬���ڵ�ʱ����Ϊdelta_t

        bool IsImplicit = true;
        bool IsSecondOrder = true;

        double EquationTime=0; //��ɢ���̶�Ӧ��ʱ�䣬��ÿ�ε���isContinueʱ��Ҫ׼������һ�μ����equationTime
        double dt=0;
        double dto=0;
        double dtoo=0;

        virtual void Initialize() = 0;
        virtual void MoveNextTimeStep() = 0;
        virtual std::unique_ptr<TransientTimeBase> GetCopy() = 0;
        void SetTransientFactor();
        void GetFactor_1stOrderExplicit(double &out_beforePhi);
        void GetFactor_2ndOrderExplicit(double &out_beforePhi, double &out_beforePhiOld, double &out_beforePhiOld2);
        void GetFactor_1stOrderImplicit(double &out_beforePhi);
        void GetFactor_2ndOrderImplicit(double &out_beforePhi, double &out_beforePhiOld, double &out_beforePhiOld2);
    };
}