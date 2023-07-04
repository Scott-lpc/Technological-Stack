% LegenSM3.m
% Legendre spectral-Galerkin method for 1D elliptic problem
% -u_yy+u=f in [0,1] with boundary condition: u(0)=1, u'(1)=0;
% exact solution: u=(1-y)^2*exp(y);  RHS: f=(2-4*y)*exp(y);
% transformed equation: -4U_xx+U=F in [-1,1]
% boundary condition: U(-1)=0, U'(1)=0;
% exact solution: U=(1/2-1/2*x)^2*exp(1/2*x+1/2)-1;
% RHS: F=-2*x*exp(1/2*x+1/2)-1.
clear all; close all;
Nvec=3:18;
% Initialization error and condition number
L2_Err=[];  condnv=[];
% Loop for various modes N to calculate numerical errors
for N=Nvec
    [xv,wv]=legs(N+1);        % Legendre-Gauss points and weights
    Lm=lepolym(N,xv);         % matrix of Legendre polynomals
    yv=1/2*(xv+1);            % variable substitution 
    U=(1-yv).^2.*exp(yv)-1;   % exact solution
    F=(2-4*yv).*exp(yv)-1;    % RHS in [0,1] 
    
    % Calculting coefficient matrix
    e1=0:N-2;  e2=0:N-3;  e3=0:N-4;
    S=diag( (4*e1+6).*(e1+1).^2./(e1+2).^2 );     % stiff matrix
    M=diag(2./(2*e1+1)+2*(2*e1+3)./(e1+2).^4+2*((e1+1)./(e1+2)).^4./(2*e1+5))...
        +diag( 2./(e2+2).^2-2*(e2+1).^2./((e2+2).^2.*(e2+3).^2) , 1 )...
        +diag(2./(e2+2).^2-2*(e2+1).^2./((e2+2).^2.*(e2+3).^2),-1)...
        +diag( -2*(e3+1).^2./((2*e3+5).*(e3+2).^2) , 2 )...
        +diag(-2*(e3+1).^2./((2*e3+5).*(e3+2).^2),-2);  % mass matrix
    A=4*S+M;
    
    % Solving the linear system
    Pm=(Lm(1:end-2,:)+diag((2*e1+3)./(e1+2).^2)...
        *Lm(2:end-1,:)-diag((e1+1).^2./(e1+2).^2)*Lm(3:end,:));
    b=Pm*diag(wv)*F;       % solving RHS
    Uh=A\b;                % expansion coefficients of u_N(x)
    Un=Pm'*Uh;             % compositing the numerical solution
    
    L2_error=sqrt(((Un-U).^2)'*wv);  % L^2 error 
    L2_Err=[L2_Err;L2_error];
    condnv=[condnv,cond(A)];         % condition number
end
% Plot L^2 error
plot(Nvec,log10(L2_Err),'bo-','MarkerFaceColor','w','LineWidth',1)
grid on
%title('L^2 error of Legendre-Galerkin method','fontsize',12)
set(gca,'fontsize',12)
xlabel('N','fontsize',14), ylabel('log_{10}Error','fontsize',14)

% sets axis tick and axis limits
xticks(2:2:18)
yticks(-16:2:0)
xlim([2 18])
ylim([-16 0])

% print -dpng -r600 LegenSM3.png
% print -depsc2 LegenSM3.eps
