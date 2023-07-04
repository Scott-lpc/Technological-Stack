% LegenSM1.m
% Legendre spectral-Galerkin method for the model equation
% -u_xx+u=f in (-1,1) with boundary condition: u(-1)=u(1)=0;
% exact solution: u=sin(kw*pi*x); 
% RHS: f=kw*kw*pi^2*sin(kw*pi*x)+sin(kw*pi*x); 
% Rmk: Use routines lepoly(); legs(); lepolym();
clear all; close all;
kw=10;
Nvec=32:2:76;
% Initialization for error
L2_Err=[]; Max_Err=[];    
% Loop for various modes N to calculate numerical errors
for N=Nvec
    [xv,wv]=legs(N+1);        % Legendre-Gauss points and weights
    Lm=lepolym(N,xv);         % matrix of Legendre polynomals
    u=sin(kw*pi*xv);          % exact solution
    f=kw*kw*pi^2*sin(kw*pi*xv)+sin(kw*pi*xv);  % RHS
    % Calculting coefficient matrix
    S=eye(N-1);               % stiff matrix
    M=diag(1./(4*(0:N-2)+6))*diag(2./(2*(0:N-2)+1)+2./(2*(0:N-2)+5))...
        -diag(2./(sqrt(4*(0:N-4)+6).*sqrt(4*(0:N-4)+14).*(2*(0:N-4)+5)),2)...
        -diag(2./(sqrt(4*(2:N-2)-2).*sqrt(4*(2:N-2)+6).*(2*(2:N-2)+1)),-2);  % mass matrix
    A=S+M;
    % Solving the linear system
    Pm=diag(1./sqrt(4*(0:N-2)+6))*(Lm(1:end-2,:)-Lm(3:end,:));  % matrix of Phi(x)
    b=Pm*diag(wv)*f;          % solving RHS
    uh=A\b;                   % expansion coefficients of u_N(x)
    un=Pm'*uh;                % compositing the numerical solution
    
    L2_error=sqrt(((un-u).^2)'*wv);  % L^2 error
    Max_error=norm(abs(un-u),inf);   % maximum pointwise error 
    L2_Err=[L2_Err;L2_error];
    Max_Err=[Max_Err;Max_error];
end
% Plot L^2 and maximum pointwise error
plot(Nvec,log10(L2_Err),'bo-','MarkerFaceColor','w','LineWidth',1)
hold on
plot(Nvec,log10(Max_Err),'rd-','MarkerFaceColor','w','LineWidth',1)
grid on
legend('L^2 error','L^{\infty} error','location','NorthEast')
set(gca,'fontsize',12)
xlabel('N','fontsize',14), ylabel('log_{10}Error','fontsize',14)

% sets axis tick and axis limits
xticks(30:10:80)
yticks(-15:3:0)
xlim([30 80])
ylim([-15 0])

% print -dpng -r600 LegenSM1.png
% print -depsc2 LegenSM1.eps
