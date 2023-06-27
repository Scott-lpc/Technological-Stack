#pragma once
#include "EquationBase.h"
#include "PressureSIMPLE.h"
#include "SIMPLEMethods.h"
#include "PhysicalFaceOverRelax.h"
namespace LxCFD
{
    class PressureCorrectionBasic : public EquationBase
    {

    public:
        SIMPLEMethods SIMPLEMethod;
        std::shared_ptr<std::vector<Vector3d>> Dc; //Dc����591���������Df,DDf��DDf��P596//Dc�ڱ����ʱ���޳��ɳڵ�Ӱ�죡
        //not local
        VelocityVector *VelocityVector;
        PressureSIMPLE *PressureSIMPLE;

        virtual void CheckCreate(FvZone &zone) override;
        void SetZone(double pressureRelaxFactor, SIMPLEMethods solvingMethod, LxCFD::VelocityVector *velocityVector, LxCFD::PressureSIMPLE *pressureInSimple, std::shared_ptr<Value1Base<double>> density);
        void SetBound_AfterSettingVelocityVector();
        virtual void Initialize() override { throw std::domain_error("Should not be called"); }
        virtual void Set_x_Bound(int taskIndex) override; //ѹ��������ʼֵ��Ϊ0
        virtual void SetAb_Step1(int taskIndex) override;
        virtual void SetAb_Step2(int taskIndex) override;
        void SetAb_PCor0Point(int CellIndex);
        virtual void Relax(int taskIndex) override {}
        virtual void Update_x_Bound(int taskIndex) override;
        void ApplyPCorToUVWP(int taskIndex);
        void ApplyPCorToUVWP_NoRelax(int taskIndex);
        void CalDc_ADiagNotRelaxed(int taskIndex); //���U V W��ϵ������δ���й��ɳ�//���Դ����ܸĶ�A_Diag�����Լ�����A_Diag����������Dc
        virtual void Setb_Step1_Cor(int taskIndex);
        virtual void Setb_Step2_Cor(int taskIndex);

    protected:
        std::vector<double> *MassFlux;
        std::vector<double> *MassImbalance;
        std::vector<double> *Pressure;
        std::shared_ptr<Value1Base<double>> Density;
        virtual void set_ALU_inner(int taskIndex);
        virtual void set_ALU_bound(IndexNumber &indexNumber, int taskIndex);
        void predict_Bound(IndexNumber &indexNumber, int taskIndex);
        void addCorrectionToUVW(IndexNumber &indexNumber, int taskIndex);
        void addCorrectionToPressure(IndexNumber &indexNumber, int taskIndex);
        void addCorrectionToPressure_NoRelax(IndexNumber &indexNumber, int taskIndex);
        virtual void addPressureCorrectionToMassFlux_inner(IndexNumber &indexNumber, int taskIndex); //��Ҫ�Թ�ʽУ�����£������ʽ������Ӧ��RhieChowһ��
        virtual void addPressureCorrectionToMassFlux_bound(IndexNumber &indexNumber, int taskIndex); //��Ҫ�Թ�ʽУ�����£������ʽ������Ӧ��RhieChowһ��
    };
}