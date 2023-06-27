#pragma once
#include "ScalarBase.h"

namespace LxCFD
{
    class MatrixADbBase : public ScalarBase
    {

    public:
        std::shared_ptr<std::vector<double>> ADiag; //���Է�����Խ���
        std::shared_ptr<std::vector<double>> b;     //���Է����� b
        std::shared_ptr<ValueBase<double>> Source;
        double RelaxFactor = 1;

    protected:
        std::vector<int> e_FixValue;
        std::vector<int> e_ALUO0;
        std::vector<int> e_Couple;

    public:
        virtual void CheckCreate(FvZone &zone) override;
        virtual void Relax(int taskIndex);

    protected:
        void clear_eFlags();
        void relax(IndexNumber &indexNumber, int taskIndex);
        void relax_bPart(IndexNumber &indexNumber, int taskIndex);
    };
}