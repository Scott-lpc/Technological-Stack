Program www_fcode_cn
  Implicit None
  Real    :: a(3,7) , r
  integer :: FILE_IN , i
  Open( NewUnit = FILE_IN , File = "text.txt" )
  Read( FILE_IN , * ) !//������һ��
  Read( FILE_IN , * ) !//�����ڶ���
  Do i = 1 , 7
    Read( FILE_IN , * ) r , a( : , i )
    write( * , * ) a(:,i)
  End Do
  Close( FILE_IN )
End Program www_fcode_cn
