% fdm1d1.m
% finite difference method for 1D problem
% -u''+u'=pi^2*sin(pi*x)+pi*cos(pi*x) in [0,1]
% u(0)=0, u(1)=0 ;
% exact solution: u=sin(pi*x)
clear all; close all;
h=0.05;
x=0:h:1;
N=length(x)-1;
A=diag((2/h^2)*ones(N-1,1))...
    +diag((1/(2*h)-1/h^2)*ones(N-2,1),1)...
    +diag((-1/(2*h)-1/h^2)*ones(N-2,1),-1);
b=pi^2*sin(pi*x(2:N))+pi*cos(pi*x(2:N));
un=A\b';
un=[0;un;0];
ue=sin(pi*x)';
plot(x,ue,'b-',x,un,'r+','LineWidth',1)
Error=max(abs(un-ue))
legend('Exact','Numerical','location','NorthEast')
%title('Finite Difference Method','fontsize',12)
set(gca,'fontsize',12)
xlabel('x','fontsize',16), ylabel('u','fontsize',16)

% print -dpng -r600 fdm1d1.png
% print -depsc2 fdm1d1.eps
