#include "BoussinesqSource.h"
#include "FvZone.h"
namespace LxCFD
{
    BoussinesqSource::BoussinesqSource(double RefT, double ThermExpanCoeff, double RefDensity, Vector3d &Gravity, FvZone &zone)
    {
        this->RefT = RefT;
        this->Total = RefDensity * ThermExpanCoeff * Gravity;
        this->ScalarT = zone.CFDFields.GetArray(ScalarTypes::Temperature);
        if (ScalarT == nullptr)
        {
            throw std::invalid_argument("����Boussinesqģ�ͳ���δ��ȡ�¶ȳ�");
        }
    }
    Vector3d BoussinesqSource::GetCellValue(int cellIndex)
    {
        return (RefT - ScalarT->at(cellIndex)) * Total;
    }
}