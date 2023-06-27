#pragma once
#include "MultiThreads.h"
#include "ScalarBase.h"
#include "VelocityVector.h"
namespace LxCFD
{
    class PressureCorrectionBasic;
    class PressureSIMPLE : public ScalarBase
    {
    public:
        //local
        double InitiialPressureProductBoundArea;
        double BoundArea;
        int CenterCellIndex;
        Vector3d Center;

        void SetZone(std::shared_ptr<Value1Base<double>> iniValue, LxCFD::VelocityVector *velocityVector, PressureCorrectionBasic *Pcor, std::shared_ptr<Value1Base<double>> density);
        virtual void Initialize() override;
        void CalPressureArea(int taskIndex); //����߽�ѹ��*���
        void CorrectPressureByReferenct(double pressureBoundCor, int taskIndex);
        void Predict_bound(int taskIndex);

    protected:
        //local
        double MinLength;
        //not local
        VelocityVector *VelocityVector;
        std::vector<double> *MassFlux;
        std::vector<Vector3d> *Dc;
        std::shared_ptr<Value1Base<double>> Density;

        void initialize_Setx_CalCenter_step1(int taskIndex);
        void initialize_step3(int taskIndex); //��������������Ҫ��Ӧ
        void predict_bound_Wall(IndexNumber &indexNumber, int taskIndex); //����2�����ȸ��ߣ��������󣬶�ӦP603�����޸ģ����ʼǣ�
        void predict_bound_byMassFlux(IndexNumber &indexNumber, int taskIndex);
    };
}