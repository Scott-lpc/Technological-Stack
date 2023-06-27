#pragma once
#define FaceCorOption 0
#include <vector>
#include "ShortArray.h"
#include "Vector3d.h"
#include "ONPair.h"

namespace LxCFD
{
	struct FvCell;
	struct FvFace
	{
	public:
		FvFace();
		// FvFace(const std::vector<int> &nodes, int c0, int c1, bool isRightHand);
		//������Ϣ
		ShortArray::Shallow NodeIDs;
		int OSideCell = 0;
		int NSideCell = 0;
		//������Ϣ
		Vector3d Center;
		Vector3d Normal; //lenthΪ������
		double Area = 0;
		Vector3d OToN; //��cell0 ����ָ��cell1���ĵ�ʸ��
		double OToNLength = 0;
		//���ڼ���
		double gOwner; //owner cell ����ľ��� ͶӰ��ownerToNeighbor��Ȼ�����ownerToNeighbor�ĳ���
		double gDiff = 0;
#if FaceCorOption == 0
		inline double GetgT()
		{
			return gDiff;
		} //�������������ʱ�õ�
#else
		double _gT = 0;
		inline double GetgT()
		{
			return _gT;
		} //�������������ʱ�õ�
#endif
		void Set(ShortArray::Deep& store, const ShortArray::Shallow& nodes, int c0, int c1, bool isRightHand);
		void CalGeometryInFace(std::vector<Vector3d>& points);
		void CalGeometryAcrossFace_inner(std::vector<FvCell>& cells);
		void CalGeometryAcrossFace_bound(std::vector<FvCell>& cells);
		Vector3d CalONGradient_DistanceWeighted(std::vector<double>& phi); //���Grad �ݶ�ʱʹ��
		ONPair GetONCellID();
		double CalAverage(std::vector<double>& phi);
		Vector3d CalAverage(std::vector<Vector3d>& gradPhi);
		Vector3d CalCFCorrectedGradient_inner(std::vector<Vector3d>& gradPhi, std::vector<double>& phi);
		Vector3d CalCFCorrectedGradient_bound(std::vector<Vector3d>& gradPhi, std::vector<double>& phi);
		Vector3d CalCFGradCorrection_inner(std::vector<Vector3d>& gradPhi, std::vector<double>& phi);
		Vector3d CalCFGradCorrection_bound(std::vector<Vector3d>& gradPhi, std::vector<double>& phi);
		double CalNonOrthogonalityCorrection_inner(std::vector<Vector3d>& gradPhi, std::vector<double>& phi);
		double CalNonOrthogonalityCorrection_bound(std::vector<Vector3d>& gradPhi, std::vector<double>& phi);
		double CalNonOrthogonalityCorrection_bound(double& out_a, double& out_b, std::vector<Vector3d>& gradPhi, std::vector<double>& phi, std::vector<FvCell>& cells); //ֻд��ƥ��minCorrection��//��Ȼ����minCorrectҲ�������ã���Ӧ�������Ƶ���ʽ������Դ�����Ի�

	private:
		void CalTriangleGeometry(Vector3d& out_CenterMultiply3, Vector3d& out_AreaMultiply2, const Vector3d& p0, const Vector3d& p1, const Vector3d& p2);
		void calO2N(std::vector<FvCell>& cells);
		Vector3d calCFCorrectPart(const Vector3d& aveGrad, std::vector<double>& phi); //���C F ����ֵ��������
		double calNonOrthogonalityCorrection(const Vector3d& aveGrad, std::vector<double>& phi);
	};
}