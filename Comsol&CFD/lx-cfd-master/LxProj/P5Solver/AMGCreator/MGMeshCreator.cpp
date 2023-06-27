#include "MGMeshCreator.h"

namespace LxCFD
{
    void MGMeshCreator::Create(int maxCoarseLevelLimit, int groupNum, int maxZoneCellCount)
    {
        if (maxCoarseLevelLimit > 0)
        {
            this->maxCoarseLevelLimit = maxCoarseLevelLimit;
        }
        else
        {
            LxConsole::Error("���������������0���Զ�����Ϊ50");
            this->maxCoarseLevelLimit = 50;
        }
        if (groupNum > 1)
        {
            zoneCreator.TargetCoarseNum = groupNum;
        }
        else
        {
            LxConsole::Error("�γɴ������ϸ���������������1�����Զ�����Ϊ4");
            zoneCreator.TargetCoarseNum = 4;
        }
        zoneCreator.InCoarseCell.resize(maxZoneCellCount);
        zoneCreator.GroupCells.resize(groupNum * 10);
        zoneCreator.tempMGCell.reserve(groupNum * 2);
        zoneCreator.coarseCells = &coarseCellsCommon;
        contactCreator.contactTempCells = &coarseCellsCommon;
    }
}
