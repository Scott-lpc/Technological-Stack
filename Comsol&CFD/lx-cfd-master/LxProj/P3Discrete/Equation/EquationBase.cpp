#include "EquationBase.h"

namespace LxCFD
{
    void EquationBase::TransientScalar::CheckCreate(EquationBase *Matrix)
    {
        this->Matrix = Matrix;
        LxTool::CheckCreateVector(RhoVolPhi_o, Matrix->InnerIndex->CellIDs.GetCount());
        LxTool::CheckCreateVector(RhoVolPhi_oo, Matrix->InnerIndex->CellIDs.GetCount());
    }

    void EquationBase::TransientScalar::SetTransient(std::shared_ptr<Value1Base<double>> trans, std::shared_ptr<Value1Base<double>> density)
    {
        x = Matrix->x.get();
        ADiag = Matrix->ADiag.get();
        b = Matrix->b.get();
        Cells = Matrix->Cells;

        Trans = trans;
        Density = density;
    }

    void EquationBase::TransientScalar::SaveOldValue(int taskIndex)
    {
        int i1, endp1;
        Matrix->InnerIndex->CellIDs.GetSeg(i1, endp1, taskIndex);
        for (; i1 < endp1; ++i1)
        {
            RhoVolPhi_o->at(i1) = Trans->GetCellValue(i1) * Density->GetCellValue(i1) * Cells->at(i1).Volumn * x->at(i1);
        }
    }

    void EquationBase::TransientScalar::PrepareToSave()
    {
        std::shared_ptr<std::vector<double>> temp = RhoVolPhi_o;
        RhoVolPhi_o = RhoVolPhi_oo;
        RhoVolPhi_oo = temp;
        switch (TransientDataState)
        {
        case TransientDataStates::None:
            TransientDataState = TransientDataStates::Only_o;
            break;
        case TransientDataStates::Only_o:
            TransientDataState = TransientDataStates::Both_o_oo;
            break;
        default:
            break;
        }
    }

    void EquationBase::TransientScalar::SetAb_ImplicitTrans(int taskIndex)
    {
        if (IsSecondOrder && TransientDataState == TransientDataStates::Both_o_oo)
        {
            int i1, endp1;
            Matrix->InnerIndex->CellIDs.GetSeg(i1, endp1, taskIndex);
            for (; i1 < endp1; ++i1)
            {
                b->at(i1) -= Factor_PhiO_2ndOrder * RhoVolPhi_o->at(i1) + Factor_PhiOO_2ndOrder * RhoVolPhi_oo->at(i1);
                ADiag->at(i1) += Factor_Phi_2ndOrder * Trans->GetCellValue(i1) * Density->GetCellValue(i1) * Cells->at(i1).Volumn;
            }
        }
        else
        {
            int i1, endp1;
            Matrix->InnerIndex->CellIDs.GetSeg(i1, endp1, taskIndex);
            for (; i1 < endp1; ++i1)
            {
                b->at(i1) += Factor_Phi_2ndOrder * RhoVolPhi_o->at(i1);
                ADiag->at(i1) += Factor_Phi_2ndOrder * Trans->GetCellValue(i1) * Density->GetCellValue(i1) * Cells->at(i1).Volumn;
            }
        }
    }

    void EquationBase::TransientScalar::SetAb_ExplicitTrans(int taskIndex)
    {
        if (IsSecondOrder && TransientDataState == TransientDataStates::Both_o_oo)
        {
            int i1, endp1;
            Matrix->InnerIndex->CellIDs.GetSeg(i1, endp1, taskIndex);
            for (; i1 < endp1; ++i1)
            {
                b->at(i1) += -ADiag->at(i1) * x->at(i1) - Factor_PhiO_2ndOrder * RhoVolPhi_o->at(i1) - Factor_PhiOO_2ndOrder * RhoVolPhi_oo->at(i1); //-calALUProductValue(ref cells[i1], x),��ʾ����ʱ����������ﲻ��
                ADiag->at(i1) = Factor_Phi_2ndOrder * Trans->GetCellValue(i1) * Density->GetCellValue(i1) * Cells->at(i1).Volumn;
            }
        }
        else
        {
            int i1, endp1;
            Matrix->InnerIndex->CellIDs.GetSeg(i1, endp1, taskIndex);
            for (; i1 < endp1; ++i1)
            {
                b->at(i1) += -ADiag->at(i1) * x->at(i1) + Factor_Phi_2ndOrder * RhoVolPhi_o->at(i1);                              //-calALUProductValue(ref cells[i1], x),��ʾ����ʱ����������ﲻ��
                ADiag->at(i1) = Factor_Phi_2ndOrder * Trans->GetCellValue(i1) * Density->GetCellValue(i1) * Cells->at(i1).Volumn; //beforePhiO��beforePhi��Ϊ�෴������˲��û�ȡ
            }
        }
    }

    void EquationBase::TransientScalar::Initialize()
    {
        MultiThreads::RunCalTasks([this](int i)
                                  { SaveOldValue(i); });
    }
    void EquationBase::CopyFromOldEquation(std::unique_ptr<EquationBase> oldEquation)
    {
        if (oldEquation == nullptr)
            return;
        x = oldEquation->x;
        IsInitialized = oldEquation->IsInitialized;
        if (oldEquation->TransData == nullptr)
            return;
        TransData = std::move(oldEquation->TransData);
    }

    void EquationBase::SetZone_TransTerm(bool isEnable, std::shared_ptr<Value1Base<double>> trans, std::shared_ptr<Value1Base<double>> density) //isEnable��ʵû��Ҫ������Ҫ�����ѣ��ڲ�����ʱTrans��densityֻ����null����
    {
        if (isEnable)
        {
            if (TransData == nullptr)
            {
                TransData = std::unique_ptr<TransientScalar>(new TransientScalar());
                TransData->CheckCreate(this);
            }
            TransData->SetTransient(trans, density);
        }
        else
        {
            TransData = nullptr;
        }
    }
}