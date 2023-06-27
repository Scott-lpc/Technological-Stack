#include "mpi.h"
#include <stdio.h>
#include<iostream>
#include <iomanip>
#include<Windows.h>
using namespace std;
//double f(double);
//double f(double x)
//{
//    return (4.0 / (1.0 + x * x));
//}
//int main(int argc, char* argv[])
//{
//    int myid, numprocs;
//    int n, i;
//    double mypi, pi;
//    double h, sum, x;
//    MPI_Init(&argc, &argv);
//    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
//    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
//    printf("Process %d of %d.\n", myid, numprocs);
//    n = 100;
//    h = 1.0 / (double)n;
//    sum = 0.0;
//    for (i = myid + 1; i <= n; i += numprocs)
//    {
//        x = h * ((double)i - 0.5);
//        sum += f(x);
//    }
//    mypi = h * sum;
//    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
//    if (myid == 0)
//    {
//        printf("The result is %.10f.\n", pi);
//    }
//    MPI_Finalize();
//}






//�Ե�ģʽ
//int main(int argc, char* argv[])
//{
//	int steps = 10;                        // ��������
//	const int totalsize = 16;              // ����ά��
//	const int mysize = 4;                  // MPI �ֿ���Ŀ, 4 �� processes
//	double A[totalsize][mysize + 2] = { 0 }; // all proc data matrix
//	double B[totalsize][mysize + 2] = { 0 }; // all proc buffer matrix
//	int begin_col = 1;                     // all proc �������ʼ��
//	int end_col = mysize + 1;              // all proc ����Ľ�����
//	int myid = 0, numprocs = 0;
//	int left = 0, right = 0;
//	int tag1 = 3, tag2 = 4; // MPI ���ͽ��� tag
//	MPI_Status status;      // MPI ״̬
//	cout << argc << endl;
//	cout << argv[0] << endl;
//	//cout << argc << endl;
//	MPI_Init(&argc, &argv);
//	double start = MPI_Wtime();
//	// MPI ��ʼ��
//	//  int thread_support = 0;
//	// MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &thread_support);
//	// ��ȡrank
//	MPI_Comm_rank(
//		MPI_COMM_WORLD /*MPI_Comm comm*/,
//		&myid /*int* size*/
//	);
//	//��ȡ������
//	MPI_Comm_size(
//		MPI_COMM_WORLD, /*MPI_Comm comm*/
//		&numprocs       /* int* size */
//	);
//	// ��ӡ������Ϣ
//	std::cout << "Process " << myid << " of " << numprocs << " is alive!" << std::endl;
//	// 4 proc ���󸳳�ֵ, ���Ҷ����������ڽ���
//	if (myid == 0)
//	{
//		cout << "id==0" << endl;
//	}
//	for (int j = 0; j < mysize + 2; j++)
//	{
//		for (int i = 0; i < totalsize; i++)
//		{
//			A[i][j] = 0.0;
//		}
//	}
//	// proc 0 �����ֵ
//	if (myid == 0)
//		for (int i = 0; i < totalsize; i++)
//		{
//			A[i][1] = 8.0;
//		}
//	// proc 3 �����ֵ
//	if (myid == 3)
//		for (int i = 0; i < totalsize; i++)
//		{
//			A[i][mysize] = 8.0;
//		}
//	// proc all ����ֵ
//	for (int i = 0; i < mysize + 2; i++)
//	{
//		A[0][i] = 8.0;
//		A[totalsize - 1][i] = 8.0;
//	}
//	// �������� proc ����
//	if (myid > 0)
//	{
//		left = myid - 1;
//	}
//	else
//	{
//		left = MPI_PROC_NULL;
//	}
//	if (myid < 3)
//	{
//		right = myid + 1;
//	}
//	else
//	{
//		right = MPI_PROC_NULL;
//	}
//	// �ſɱȵ��� ���岽��
//	for (int i = 0; i < steps; i++)
//	{
//		// �󶨷��ͽ���, proc:��4��  -> rihgt proc: ��0��
//		MPI_Sendrecv(
//			&A[0][mysize], // onst void *sendbuf; ���Ҳ�����, Ҫ���͸���һ�� proc
//			totalsize,     // int sendcount; ���ݳ���
//			MPI_DOUBLE,    // MPI_Datatype  sendtype; ��������
//			right,         // int dest
//			tag1,          // int sendtag,
//			//---------------------------------------
//			&A[0][0],       // void *recvbuf; ��������λ��
//			totalsize,      // int recvcount; ���ճ���
//			MPI_DOUBLE,     // MPI_Datatype  recvtype; ��������
//			left,           // int source; ����Դ
//			tag1,           // int recvtag; ���� tag
//			MPI_COMM_WORLD, // MPI_Comm comm; ͨ����
//			&status         // MPI_Status *status; ���ͻ����״̬;
//		);
//		// �󶨷��ͽ���, proc:��1��  -> left proc: ��5��
//		MPI_Sendrecv(
//			&A[0][1],   // onst void *sendbuf
//			totalsize,  // int sendcount
//			MPI_DOUBLE, // MPI_Datatype sendtype
//			left,       // int dest
//			tag2,       // int sendtag,
//			//---------------------------------------
//			&A[0][mysize + 1], // void *recvbuf,
//			totalsize,         // int recvcount,
//			MPI_DOUBLE,        // MPI_Datatype recvtype,
//			right,             // int source,
//			tag2,              // int recvtag,
//			MPI_COMM_WORLD,    // MPI_Comm comm,
//			&status            // MPI_Status *status
//		);
//		// proc 0
//		if (myid == 0)
//		{
//			begin_col = 2; // 0th ���ǻ���, 1st ��ȱ�����, ���Դ� 2nd ��ʼ
//		}
//		// proc 3
//		if (myid == 3)
//		{
//			end_col = mysize - 1; //��������
//		}
//		// ��ÿ�� proc matrix �ڲ�, ȥ����λ��, �����ڲ���
//		for (int j = begin_col; j < end_col; j++)
//		{
//			// in each proc matrix, ȥ����λ��, �����ڲ���
//			for (int i = 1; i < totalsize - 1; i++)
//			{
//				B[i][j] = (A[i][j + 1] + A[i][j - 1] + A[i + 1][j] + A[i - 1][j]) * 0.25;
//			}
//		}
//		// �����ݴӻ������ ��ȡ�� ԭ����
//		for (int j = begin_col; j < end_col; j++)
//		{
//			for (int i = 1; i < totalsize - 1; i++)
//			{
//				A[i][j] = B[i][j];
//			}
//		}
//	}
//	// ��ӡ�������
//	for (int row = 1; row < totalsize - 1; row++)
//	{
//		std::cout << "proc (" << myid << "):  ";
//		for (int col = begin_col; col < end_col; col++)
//		{
//			std::cout << std::setiosflags(std::ios_base::left)
//				<< std::setw(15) << A[row][col]
//				<< std::resetiosflags(std::ios_base::left);
//		}
//		std::cout << std::endl;
//	}
//	// MPI ��β
//	double end = MPI_Wtime();
//	char name[] = "";
//	int result;
//	MPI_Get_processor_name(name, &result);
//	cout << name << "  " << result << endl;
//
//
//	MPI_Group worldGroup;
//	MPI_Comm_group(MPI_COMM_WORLD, &worldGroup);
//	MPI_Group group1, group2;
//	const int group1p[2] = { 0,1 };
//	const int group2p[2] = { 2,3 };
//	MPI_Group_incl(worldGroup, 2, &group1p[0], &group1);
//	MPI_Group_incl(worldGroup, 2, &group2p[0], &group2);
//	MPI_Comm comm1, comm2;
//	MPI_Comm_create(MPI_COMM_WORLD, group1, &comm1);
//	MPI_Comm_create(MPI_COMM_WORLD, group2, &comm2);
//
//	//MPI_Abort(comm2, 99);
//	if (myid == 0)
//	{
//		exit(0);
//	}
//	Sleep(10000000);
//	
//	MPI_Finalize();
//	system("pause");
//	return 0;
//}





//����ģʽ
#include <algorithm>

// ȫ�ֱ�����ʼ��
const int rows = 100;
const int cols = 100;
double a[rows][cols] = { 0 }; //���� [rows,cols]
double b[cols] = { 0 };       // �о���
double c[rows] = { 0 };       // �洢�������
double buffer[cols] = { 0 };  //�������
int numsent = 0;            // �ѷ�������
int numprocs = 0;           //���ý�����
const int m_id = 0;         // ������ id
const int end_tag = 0;      // ��־������ɵ� tag
MPI_Status status;          // MPI ״̬

// ��������Ҫ��������: ��������, �������ݺͽ��ռ�����
void master()
{
    // 1׼������
    for (int i = 0; i < cols; i++)
    {
        b[i] = i;
        for (int j = 0; j < rows; j++)
        {
            a[i][j] = i + 1;
        }
    };
    // 2 ÿ���ӽ��̶�������ͬ�ľ���b,
    // ����ʹ�ù㲥����, �������̽�����b��ͨ���������н��̹㲥һ�¾���b,
    // Ȼ��ӽ��̾Ϳ��Զ����յ�����b��һ������.
    MPI_Bcast(
        b,             // void* buffer
        cols,          // int count
        MPI_DOUBLE,    // MPI_Datatype datatype
        m_id,          // int root,
        MPI_COMM_WORLD // MPI_Comm comm
    );
    // ����A������֮ǰ�ᵽ���� MPI_SEND, ����ÿ�е�����
    // ʵ�ʵ��ӽ������� numprocs - 1 ��
    for (int i = 0; i < min(numprocs - 1, rows); i++)
    {
        for (int j = 0; j < cols; j++)
        {
            buffer[j] = a[i][j];
        }
        //���;���A ��������, ʹ�þ���������Ϊ tag MPI_DOUBLE,
        MPI_Send(
            buffer,        // const void* buf,
            cols,          // int count,
            MPI_DOUBLE,    // MPI_Datatype datatype,
            i + 1,         // int dest, 0 �з��� rank 1, �Դ�����
            i + 1,         // int tag, row number +1
            MPI_COMM_WORLD // MPI_Comm comm
        );
        numsent = numsent + 1; // ��¼�ѷ��͵�����
    };
    // 3 ��ִ���귢�Ͳ����, ��Ҫ���������ջ�
    double ans = 0.0;           // �洢�����Ԫ��
    for (int i = 0; i < rows; i++)
    {
        MPI_Recv(
            &ans,           // void* buf,
            1,              // int count,
            MPI_DOUBLE,     // MPI_Datatype datatype,
            MPI_ANY_SOURCE, // int source,
            MPI_ANY_TAG,    // int tag,
            MPI_COMM_WORLD, // MPI_Comm comm,
            &status         // MPI_Status * status
        );
        // sender ���ڼ�¼�Ѿ���������ͻ������̵Ĵӽ��̺�
        int sender = status.MPI_SOURCE;
        //�ڷ���ʱ, ����ע�� tag = ������к�+1,
        int rtag = status.MPI_TAG - 1;
        c[rtag] = ans; //�� c(rtag)=ans���ڶ�Ӧλ�ô洢���
        // numsent ���ѷ�����, �����ж��Ƿ�����������
        // �����Ѿ����ͻ�������, ���ɴ���ôӽ����Ѿ����ڿ���״̬
        // ��֮��ķ�����, ������еĽ��̼������ͼ�������
        if (numsent < rows)
        {
            // ��ȡ��һ��
            for (int j = 0; j < cols; j++)
            {
                buffer[j] = a[numsent][j];
            }
            MPI_Send(
                buffer, cols, MPI_DOUBLE,
                sender, numsent + 1, MPI_COMM_WORLD);
            numsent = numsent + 1;
        }
        //����������֮��, ��ӽ��̷���һ������Ϣ,
        //�ӽ��̽��յ�����Ϣʱ, ��ִ��MPI_FINALIZE������.
        else
        {
            int tmp = 1.0;
            MPI_Send(
                &tmp, 0, MPI_DOUBLE,
                sender, end_tag, MPI_COMM_WORLD);
        }
    };
}
//
//// �ӽ���
void slave()
{
    //�ӽ���������Ҫ���������̹㲥�ľ���b
    MPI_Bcast(b, cols, MPI_DOUBLE, m_id, MPI_COMM_WORLD);
    while (1)
    {
        MPI_Recv(
            buffer, cols, MPI_DOUBLE,
            m_id, MPI_ANY_TAG, MPI_COMM_WORLD,
            &status);
        //ֱ������A�������ж�������ɺ�, �����̻ᷢ�� tag Ϊ end_tag �Ŀ���Ϣ,
        if (status.MPI_TAG != end_tag)
        {
            int row = status.MPI_TAG;
            double ans = 0.0;
            for (int i = 0; i < cols; i++)
            {
                ans = ans + buffer[i] * b[i];
            }
            MPI_Send(
                &ans, 1, MPI_DOUBLE,
                m_id, row, MPI_COMM_WORLD);
        }
        else {
            break;
        }
    }
}

int main(int argc, char* argv[])
{
    //----------------------------------
    // MPI ��ʼ��
    MPI_Init(&argc, &argv);
    //��ȡ��������
    MPI_Comm_size(
        MPI_COMM_WORLD, // MPI_Comm comm
        &numprocs       // int* size
    );
    // ��ȡrank
    int myid = 0; // rank number
    MPI_Comm_rank(
        MPI_COMM_WORLD, // MPI_Comm comm,
        &myid           // int* size
    );
    // ��ӡ������Ϣ
    std::cout << "Process " << myid << " of " << numprocs << " is alive!" << std::endl;
    //----------------------------------
    if (myid == m_id)
    {
        master(); //�����̵ĳ������
    }
    else
    {
        slave(); //�ӽ��̵ĳ������
    }
    // ��ӡ���
    if (myid == m_id) {
        for (int i = 0; i < rows; i++)
        {
            std::cout << "the ele (" << i << "): "
                << std::setiosflags(std::ios_base::right)
                << std::setw(15) << c[i]
                << std::resetiosflags(std::ios_base::right)
                << std::endl;
        }
    }
    // MPI ��β
    MPI_Finalize();
}