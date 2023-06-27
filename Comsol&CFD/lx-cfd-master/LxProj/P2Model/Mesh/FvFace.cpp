#include "FvFace.h"
#include <cmath>
#include "LxConsole.h"
#include "FvCell.h"
namespace LxCFD
{
    FvFace::FvFace() {}
    void FvFace::Set(ShortArray::Deep &store, const ShortArray::Shallow &nodes, int c0, int c1, bool isRightHand)
    {
        OSideCell = c0;
        NSideCell = c1;
        NodeIDs = store.Create(nodes);
        if (!isRightHand)
        {
            NodeIDs.Reverse();
        }
    }
    void FvFace::CalGeometryInFace(std::vector<Vector3d> &points)
    {
        Center = Vector3d();
        Normal = Vector3d();
        double areaWeightSum = 0; //����⼸�������治��һ��ƽ�棬�����ڳ���
        if (NodeIDs.GetLengh() < 3)
        {
            LxConsole::Error("������ı���С��3");
            return;
        }

        for (int i1 = 2; i1 < NodeIDs.GetLengh(); ++i1)
        {
            Vector3d CenterMultiply3;
            Vector3d NormalMultiply2;
            CalTriangleGeometry(CenterMultiply3, NormalMultiply2,
                                points.at(NodeIDs.at(0)),
                                points.at(NodeIDs.at(i1 - 1)),
                                points.at(NodeIDs.at(i1)));
            double areaWeight = NormalMultiply2.CalLength();
            areaWeightSum += areaWeight;
            Center += (areaWeight * CenterMultiply3);
            Normal += (NormalMultiply2);
        }
        Normal *= (0.5);
        Area = Normal.CalLength();
        Center *= (1 / (3 * areaWeightSum));
        if (Area == 0)
        {
            LxConsole::Error("�������Ϊ0����");
        }
    }

    void FvFace::CalGeometryAcrossFace_inner(std::vector<FvCell> &cells)
    {
        calO2N(cells);
        if (OToNLength == 0)
        {
            gOwner = 0.5;
            return;
        } //��������ʾ����
        Vector3d ownerToFace = Center - cells.at(OSideCell).Center;
        gOwner = Vector3d::DotProduct(ownerToFace, OToN) / OToN.CalLengthSquared();
        if (gOwner > 1)
        {
            gOwner = 1;
        }
        else if (gOwner < 0)
        {
            gOwner = 0;
        }
    }

    void FvFace::CalGeometryAcrossFace_bound(std::vector<FvCell> &cells)
    {
        calO2N(cells);
        gOwner = 1;
    }
    Vector3d FvFace::CalONGradient_DistanceWeighted(std::vector<double> &phi) //���Grad �ݶ�ʱʹ��
    {
        double deltaPhiWithDistace = (phi.at(NSideCell) - phi.at(OSideCell)) / OToNLength;
        return deltaPhiWithDistace * OToN;
    }
    ONPair FvFace::GetONCellID()
    {
        return ONPair(OSideCell, NSideCell);
    }
    double FvFace::CalAverage(std::vector<double> &phi)
    {
        return phi.at(OSideCell) * (1 - gOwner) + phi.at(NSideCell) * gOwner;
    }
    Vector3d FvFace::CalAverage(std::vector<Vector3d> &gradPhi)
    {
        return (1 - gOwner) * gradPhi.at(OSideCell) + gOwner * gradPhi.at(NSideCell);
    }
    Vector3d FvFace::CalCFCorrectedGradient_inner(std::vector<Vector3d> &gradPhi, std::vector<double> &phi)
    {
        Vector3d aveGrad = CalAverage(gradPhi);
        Vector3d cor = calCFCorrectPart(aveGrad, phi);
        return aveGrad + cor;
    }
    Vector3d FvFace::CalCFCorrectedGradient_bound(std::vector<Vector3d> &gradPhi, std::vector<double> &phi)
    {
        Vector3d aveGrad = gradPhi.at(OSideCell);
        Vector3d cor = calCFCorrectPart(aveGrad, phi);
        return aveGrad + cor;
    }
    Vector3d FvFace::CalCFGradCorrection_inner(std::vector<Vector3d> &gradPhi, std::vector<double> &phi)
    {
        Vector3d aveGrad = CalAverage(gradPhi);
        return calCFCorrectPart(aveGrad, phi);
    }

    Vector3d FvFace::CalCFGradCorrection_bound(std::vector<Vector3d> &gradPhi, std::vector<double> &phi)
    {
        Vector3d aveGrad = gradPhi.at(OSideCell);
        return calCFCorrectPart(aveGrad, phi);
    }
    double FvFace::CalNonOrthogonalityCorrection_inner(std::vector<Vector3d> &gradPhi, std::vector<double> &phi) //��ʽ����
    {
        return calNonOrthogonalityCorrection(CalAverage(gradPhi), phi);
    }

    double FvFace::CalNonOrthogonalityCorrection_bound(std::vector<Vector3d> &gradPhi, std::vector<double> &phi)
    {
        return calNonOrthogonalityCorrection(gradPhi.at(OSideCell), phi);
    }

    double FvFace::CalNonOrthogonalityCorrection_bound(double &out_a, double &out_b, std::vector<Vector3d> &gradPhi, std::vector<double> &phi, std::vector<FvCell> &cells) //ֻд��ƥ��minCorrection��//��Ȼ����minCorrectҲ�������ã���Ӧ�������Ƶ���ʽ������Դ�����Ի�
    {
        out_a = (pow(Area, 2) - pow(GetgT() * OToNLength, 2)) / cells.at(OSideCell).Volumn;
        double cor = calNonOrthogonalityCorrection(gradPhi.at(OSideCell), phi);
        out_b = cor - out_a * (phi.at(NSideCell) - phi.at(OSideCell));
        return cor;
    }
    //private
    void FvFace::CalTriangleGeometry(Vector3d &out_CenterMultiply3, Vector3d &out_AreaMultiply2, const Vector3d &p0, const Vector3d &p1, const Vector3d &p2)
    {
        Vector3d v01 = p1 - p0;
        Vector3d v12 = p2 - p1;
        out_AreaMultiply2 = Vector3d::CrossProduct(v01, v12);
        out_CenterMultiply3 = Vector3d(p0.X + p1.X + p2.X, p0.Y + p1.Y + p2.Y, p0.Z + p1.Z + p2.Z);
    }

    void FvFace::calO2N(std::vector<FvCell> &cells)
    {
        OToN = cells.at(NSideCell).Center - cells.at(OSideCell).Center;
        OToNLength = OToN.CalLength();
        double normalDotOToN = Vector3d::DotProduct(Normal, OToN);
        if (normalDotOToN < 0)
        {
            LxConsole::Error("���Owner��ϵ����ĳ���һ�£������������������");
        }
        if (OToNLength == 0)
        {
            LxConsole::Error("���������ص����������ִ���");
        }
        //---------------------�޸ĵ�1------------------------------------
#if FaceCorOption == 0
        //!!!!!!!!!!!����ΪminCorrection
        this->gDiff = normalDotOToN / pow(OToNLength, 2);
#else
        //!!!!!!!!!!!����ΪoverCorrection�޸ĵ�1
        this->gDiff = pow(Area, 2) / normalDotOToN;
        this->_gT = normalDotOToN / pow(OToNLength, 2);
#endif
    }
    Vector3d FvFace::calCFCorrectPart(const Vector3d &aveGrad, std::vector<double> &phi) //���C F ����ֵ��������
    {
        return (phi.at(NSideCell) - phi.at(OSideCell) - Vector3d::DotProduct(aveGrad, OToN)) / pow(OToNLength, 2) * OToN;
    }
    double FvFace::calNonOrthogonalityCorrection(const Vector3d &aveGrad, std::vector<double> &phi)
    {
        //----------------------------�޸ĵ�2-----------------------------------
#if FaceCorOption == 0
        //!!!!!!!!����ΪminCorrection
        return Vector3d::DotProduct(aveGrad, Normal - gDiff * OToN);
#else
        //!!!!!!!!!����ΪoverCorrection
        return Vector3d.DotProduct(aveGrad, Normal - _gT * OToN) + (phi->at(NSideCell) - phi->at(OSideCell)) * (_gT - gDiff);
#endif
    }
}

// FvFace::FvFace(const std::vector<int> &nodes, int c0, int c1, bool isRightHand) : NodeIDs(nodes), OSideCell(c0), NSideCell(c1)
// {
//     if (!isRightHand)
//     {
//         NodeIDs.Reverse();
//     }
// }