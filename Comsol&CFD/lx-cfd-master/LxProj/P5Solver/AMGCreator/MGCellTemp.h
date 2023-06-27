#pragma once
#include "MGFaceTemp.h"
#include "MGCell.h"
namespace LxCFD
{
    class MGCellTemp
    {
    public:
        //MGCellTemp();
        LinkArray<int>::List FineCells;
        LinkArray<int>::List InnerFineFaces;
        LinkArray<int>::List FacesAtO;
        LinkArray<int>::List FacesAtN;
        LinkArray<MGFaceTemp>::List CoarseOFacesStorage; //�����Ĵ�Face�ȴ洢������
    };
}
//std::vector<MGFaceTemp> CoarseOFacesStorage;
// std::vector<int> CellFacesAsO;
// std::vector<int> CellFacesAsN;