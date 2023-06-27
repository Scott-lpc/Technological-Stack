#include "SolidCubeCase.h"
#include "FluentMesh.h"
#include "AppRes.h"
#include "VTUWriter.h"
#include "TestMonitor.h"
#include "ExcuteSaveFile.h"
namespace LxCFD
{
    SolidCubeCase::SolidCubeCase(int ID, const std::string &fileFolder) : ExampleBase(ID, fileFolder)
    {
        Description = "˲̬���嵼��";
    }

    void SolidCubeCase::RunCase()
    {
        std::string fileName = "Case0-Chang";
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
                physical->SetPhysicalType(PhysicalTypes::Solid);
                physical->IsEnergyEnabled = true;
                AppRes::PhysicalCenter->PhysicalModels.push_back(physical);
                //3.���ñ߽�����
                FvZone &zone = FineZone("BODY");
                zone.PhysicalModel = physical;

                FvThread &thread_in = FindThread(zone, "IN");
                thread_in.WallThermalType = WallThermalTypes::FixHeatFlux;
                thread_in.HeatFlux = std::unique_ptr<Value1Base<double>>(new Value1Const::Double(3000));
                FvThread &thread_out = FindThread(zone, "OUT");
                thread_out.WallThermalType = WallThermalTypes::FixTemperature;
                thread_out.Temperature = std::unique_ptr<Value1Base<double>>(new Value1Const::Double(300));

                AppRes::FvMesh->IsTransient = true;
                auto transientSetting = std::unique_ptr<TransientTimeFixed>(new TransientTimeFixed());
                transientSetting->FixInterval = 3600;
                transientSetting->IsImplicit = true;
                transientSetting->IsSecondOrder = true;
                AppRes::FvMesh->TransientTimeSetting = std::move(transientSetting);
                //3.���м������
                MultiThreads::SetThreadsCout(2);
                AppRes::StepsCenter->SteadyStep.MaxSteps = 30;
                AppRes::StepsCenter->TransientStep.MaxSteps = 30;
                std::unique_ptr<UserMonitorBase> monitor = std::unique_ptr<UserMonitorBase>(new TestMonitor());
                AppRes::MonitorCenter->UserMonitors.push_back(std::move(monitor));
                //4.�����ļ�����
                std::shared_ptr<ExcuteSaveFile> saveFileTask = std::shared_ptr<ExcuteSaveFile>(new ExcuteSaveFile());
                saveFileTask->FilePath = FileFolder + fileName + ".vtu";
                saveFileTask->ExportScalars.push_back(ScalarTypes::Temperature);
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