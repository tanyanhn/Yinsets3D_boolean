a = [0 0 0];
b = [0 1 0];
c = [0 1 1];
d = [0 0 1];
p = [1 0 0;
     1 1 0;
     0 0 0.7;
     1.5 1.5 1.5];
p1 = p([1 2 3],:);
p2 = p([1 2 4],:);
patch(p1(:,1),p1(:,2),p1(:,3),'r'); % 三角形，涂成红色，[1 0 0]表示RGB颜色值中的红色
hold on;
patch(p2(:,1),p2(:,2),p2(:,3),'b'); 

seg = p1(1,:) - p1(2,:);
v1 = p1(3,:) - p1(1,:);
v2 = p2(3,:) - p2(2,:);
d1 = cross(seg,v1);
d1 = d1/norm(d1);
d2 = cross(-seg,v2);
d2 = d2/norm(d2);

ep = [1,0.2,0.4];
quiver3(ep(1),ep(2),ep(3),d1(1),d1(2),d1(3),'r','LineWidth',1,'MaxHeadSize',0.3)
quiver3(ep(1),ep(2),ep(3),d2(1),d2(2),d2(3),'b','LineWidth',1,'MaxHeadSize',0.3)

axis equal

figure

patch(p1(:,1),p1(:,2),p1(:,3),'r'); % 三角形，涂成红色，[1 0 0]表示RGB颜色值中的红色
hold on;
patch(p2(:,1),p2(:,2),p2(:,3),'b'); 

seg = p1(1,:) - p1(2,:);
v1 = p1(3,:) - p1(1,:);
v2 = p2(3,:) - p2(2,:);
d1 = cross(seg,v1);
d1 = -d1/norm(d1);
d2 = cross(-seg,v2);
d2 = -d2/norm(d2);


quiver3(ep(1),ep(2),ep(3),d1(1),d1(2),d1(3),'r','LineWidth',1,'MaxHeadSize',0.3)
quiver3(ep(1),ep(2),ep(3),d2(1),d2(2),d2(3),'b','LineWidth',1,'MaxHeadSize',0.3)

axis equal

% figure
% t = 0:0.01:1;
% x = 0:0.01:1;
% y = 0:0.01:1;
% plot(x+1,2*y); hold on;
% 
% plot(-x+1,y); hold on;