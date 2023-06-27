Module DFile_Mod
  Implicit None
  
contains 

  Integer Function GetDataN( cStr )
    Character( Len = * ) , Intent( IN ) :: cStr
    Integer :: i
    Logical :: bIsSeparator , bIsQuote
    GetDataN = 0
    bIsSeparator = .TRUE.
    bIsQuote = .FALSE.
    Do i = 1 , Len_Trim( cStr )
      Select Case( cStr(i:i) )
      Case( '"' , "'" ) !// �����������
        If ( .Not.bIsQuote ) GetDataN = GetDataN + 1  !//������������У�������һ������
        bIsQuote = .Not.bIsQuote !// ���Ž�����ʼ
        bIsSeparator = .FALSE.
      Case( " " , "," , char(9) ) !// ��������ָ���
        If ( .Not.bIsQuote ) then  !// �ָ����������������
          bIsSeparator = .TRUE.
        End If
      Case Default      
        If ( bIsSeparator ) then
          GetDataN = GetDataN + 1
        End If
        bIsSeparator = .FALSE.
      End Select
    End Do
  End Function GetDataN
  
  Integer Function GetFileN( iFileUnit )
  !// �˺���Ӧ�ڴ��ļ����������á����ú��ȡλ�÷����ļ���ʼλ��
    Implicit None
    Integer , Intent( IN ) :: iFileUnit
    character( Len = 1 ) :: cDummy
    integer :: ierr
    GetFileN = 0
    Rewind( iFileUnit )
    Do
      Read( iFileUnit , * , ioStat = ierr ) cDummy
      If( ierr /= 0 ) Exit
      GetFileN = GetFileN + 1
    End Do
    Rewind( iFileUnit )
  End Function GetFileN 

End Module DFile_Mod

Program www_fcode_cn
  Use DFile_Mod
  Implicit None
  Real , allocatable :: a(:,:)
  Character(len=512) :: cStr
  Real    :: r  
  integer :: FILE_IN , i , nRow , nCol
  Open( NewUnit = FILE_IN , File = "text.txt" )
  nRow = GetFileN( FILE_IN ) - 2 !//����ļ����� 
  Read( FILE_IN , * ) !//������һ��
  Read( FILE_IN , * ) !//�����ڶ���
  Read( FILE_IN , '(a512)' ) cStr!//��ȡ������
  nCol = GetDataN( cStr ) - 1 !//��õ������ж�����
  write( *, '("Row:",g0," Col: ",g0)' ) nRow , nCol
  Allocate( a( nCol , nRow ) )
  Backspace( FILE_IN ) !//�˻ص�������
  Do i = 1 , nRow
    Read( FILE_IN , * ) r , a( : , i )
    write( * , * ) a(:,i)
  End Do
  Deallocate( a )
  Close( FILE_IN )
End Program www_fcode_cn
