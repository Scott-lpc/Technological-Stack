#pragma once
#include "EquationBase.h"
#include <algorithm>

namespace LxCFD
{
	class NSEquationBase : public EquationBase
	{
	public:
		class ICalgCfPhir
		{
		public:
			virtual double cal(double r, double gCf) = 0; //������λ��ΪphiC+�������*(PhiD-PhiC)
		};
		class MUSCL_DC : public ICalgCfPhir
		{
		public:
			virtual double cal(double r, double gCf) override;
		};
		class OSHER_DC : public ICalgCfPhir
		{
		public:
			virtual double cal(double r, double gCf) override;
		};
		std::shared_ptr<Value1Base<double>> Conv;  //������ǰ���ϵ������������Ϊ1��������������Ϊ������Cp
		std::shared_ptr<Value1Base<double>> Gamma; //Tau��Continuum���ã����ڸ�Zone��ֵ�в��죬������һ�ݣ�������Դ�����ٵ����洢
	protected:
		std::vector<double> *MassImbalance;
		std::vector<double> *MassFlux;
		std::vector<float> *VelocityDirectionCorrection; //��������,���ʼ�
		std::unique_ptr<ICalgCfPhir> CalgCfPhir;

		void set_x_flow(Value1Base<double> &baseValue, IndexNumber &indexNumber, int taskIndex);
		double calUpwindValue_faceFluxPositive(FvFace &face, double phiO, double phiN, double velDirCor);
		double calUpwindValue_faceFluxNegative(FvFace &face, double phiO, double phiN, double velDirCor);
		void set_Ab_Upwind_inner(int taskIndex);
		void set_Ab_SOU_inner(int taskIndex);
		void set_Ab_OSHER_inner(int taskIndex);
		void set_Ab_MUSCL_inner(int taskIndex);
		void set_Ab_TVD_DC_inner(int taskIndex);
		void set_Ab_boundOwner_DiffPart(IndexNumber &indexNumber, int taskIndex);
		void set_A_boundOwner_0(IndexNumber &indexNumber, int taskIndex);
		void set_Ab_bound_flow(IndexNumber &indexNumber, int taskIndex);
		void setAb_step2(Value1Base<double> *source1, int taskIndex);
		void setAb_step2(Value2Base<double> *source2, int taskIndex);
	};
}