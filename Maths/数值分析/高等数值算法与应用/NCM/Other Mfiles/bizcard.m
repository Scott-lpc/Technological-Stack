function bizcard
% BIZCARD  Future version of The MathWorks business card.
%   Click anywhere on the card.

clf reset
shg
set(gcf,'name','The MathWorks Business Card', ...
   'menu','none','numbertitle','off');
ax = axes('pos',[.125 .25 .75 .5],'xtick',[],'ytick',[],'box','on');
text(.3,.8,'The MathWorks','fontsize',24, ...
   'fontname','Times New Roman','fontweight','normal');
text(.5,.6,'CLEVE MOLER','fontsize',12);
text(.5,.5,'Chief Scientist','fontsize',12);
text(.5,.4,'moler@mathworks.com','fontsize',12);
text(.5,.3,'508.647.7000','fontsize',12);
text(.1,.17,'www.mathworks.com','fontsize',10);
text(.1,.1,'The MathWorks, Inc. 3 Apple Hill Drive, Natick, MA 01760', ...
   'fontsize',10);
L{1} = 30*membrane(1,25);
L{2} = 2*membrane(2,25);
L{3} = -2*membrane(3,25);
L{4} = 5*membrane(4,25);
L{5} = -3*membrane(5,25);
L{6} = 4*membrane(6,25);
axes('Position',[.15 .55 .2 .2], ...
     'CameraPosition', [-193.4013 -265.1546  220.4819],...
     'CameraTarget',[26 26 10], ...
     'CameraUpVector',[0 0 1], ...
     'CameraViewAngle',9.5, ...
     'DataAspectRatio', [1 1 .9],...
     'Visible','off', ...
     'XLim',[1 51], ...
     'YLim',[1 51], ...
     'ZLim',[-13 40]);
s = surface(L{1}, ...
     'EdgeColor','none', ...
     'FaceColor',[0.9 0.2 0.2], ...
     'FaceLighting','phong', ...
     'AmbientStrength',0.3, ...
     'DiffuseStrength',0.6, ... 
     'Clipping','off',...
     'BackFaceLighting','lit', ...
     'SpecularStrength',1.1, ...
     'SpecularColorReflectance',1, ...
     'SpecularExponent',7);
light('Position',[40 100 20], ...
     'Style','local', ...
     'Color',[0 0.8 0.8]);
light('Position',[.5 -1 .4], ...
     'Color',[0.8 0.8 0]);

mu = sqrt([9.6397238445, 15.19725192, 2*pi^2, ...
           29.5214811, 31.9126360, 41.4745099]);
set(gcf,'userdata',false);
set(ax,'buttondownfcn','set(gcf,''userdata'',~get(gcf,''userdata''))');
stop = uicontrol('style','toggle','string','X','fontweight','bold', ...
   'units','norm','pos',[.95 .95 .05 .05]);

t = 0;
delta = .025;
while ~get(stop,'value')
   Z = cos(mu(1)*t)*L{1} + sin(mu(2)*t)*L{2} + sin(mu(3)*t)*L{3} +  ...
       sin(mu(4)*t)*L{4} + sin(mu(5)*t)*L{5} + sin(mu(6)*t)*L{6};
   set(s,'zdata',Z)
   pause(.01)
   if get(gcf,'userdata')
      t = t + delta;
   end
end
close(gcf)
