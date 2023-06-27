#include "CavityFlow.h"
#include "FluentMesh.h"
#include "ExcuteSaveFile.h"
namespace LxCFD
{
    CavityFlow::CavityFlow(int ID, const std::string &fileFolder) : ExampleBase(ID, fileFolder)
    {
        Description = "��ǻ����������";
    }
    void CavityFlow::RunCase()
    {
        std::string fileName = "Case2-Cavity";
        try
        {
            //1.��ȡ�ļ�
            {
                FluentMesh fmesh;
                fmesh.ReadFile(FileFolder + fileName + ".msh");
            }
            AppRes::FvMesh->PrepareMeshData();
            {
                //2.��������ģ��
                auto physical = std::shared_ptr<PhysicalModel>(new PhysicalModel());
                physical->SetPhysicalType(PhysicalTypes::Fluid);
                physical->IsEnergyEnabled = false;
                AppRes::PhysicalCenter->PhysicalModels.push_back(physical);
                //3.���ñ߽�����
                FvZone &zone = FineZone("SOLID");
                zone.PhysicalModel = physical;

                FvThread &thread_UP = FindThread(zone, "UP");
                thread_UP.BoundaryType = BoundaryTypes::FluidWall;
                thread_UP.IsStaticWall = false;
                thread_UP.Velocity = std::unique_ptr<Value1Base<Vector3d>>(new Value1Const::Vector(Vector3d(1, 0, 0)));
                FvThread &thread_DOWN = FindThread(zone, "DOWN");
                thread_DOWN.BoundaryType = BoundaryTypes::FluidWall;
                FvThread &thread_LEFT = FindThread(zone, "LEFT");
                thread_LEFT.BoundaryType = BoundaryTypes::FluidWall;
                FvThread &thread_RIGHT = FindThread(zone, "RIGHT");
                thread_RIGHT.BoundaryType = BoundaryTypes::FluidWall;
                FvThread &thread_FRONT = FindThread(zone, "FRONT");
                thread_FRONT.BoundaryType = BoundaryTypes::Symmetry;
                FvThread &thread_BACK = FindThread(zone, "BACK");
                thread_BACK.BoundaryType = BoundaryTypes::Symmetry;
                //3.���м������
                MultiThreads::SetThreadsCout(1);
                AppRes::StepsCenter->SteadyStep.MaxSteps = 300;
                //4.�����ļ�����
                std::shared_ptr<ExcuteSaveFile> saveFileTask = std::shared_ptr<ExcuteSaveFile>(new ExcuteSaveFile());
                saveFileTask->FilePath = FileFolder + fileName + ".vtu";
                saveFileTask->ExportScalars = std::vector<ScalarTypes>{ScalarTypes::Temperature, ScalarTypes::Pressure,ScalarTypes::PressureCorrection,ScalarTypes::MassImbalance, ScalarTypes::Velocity};
                AppRes::ExcuteCenter->ExcuteAfterAll_Fixed.push_back(saveFileTask);
                //5.ִ�м���
                AppRes::ComputeTask->RunComputation();
            }
        }
        catch (std::exception &e)
        {
            LxConsole::Error(e.what());
            return;
        }
    }
}