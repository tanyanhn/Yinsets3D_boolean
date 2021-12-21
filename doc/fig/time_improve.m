clc
clear all
close all

x = [2112, 10880, 34560, 114176, 359424, 539392];
lx = log(x);
y1 = [0.302020 1.549209 3.878483 9.649845 25.118267 35.477708];
ly1 = log(y1);
y2 = [0.175366 1.140628 4.671456 22.934571 129.951804 306.352480];
ly2 = log(y2);
plot(lx, ly1,'red-o');hold on
plot(lx, ly2,'blue-o');
xlabel('log(n)');
ylabel('log(t)');
legend('空间划分法','两两求交法');

%  num of tris:(960, 1152), 2112, Octree deep:3
% OctreeIntersect cpu time :  0.302020s wall, 0.300000s user + 0.000000s system = 0.300000s CPU (99.3%)
% normalIntersect cpu time :  0.175366s wall, 0.180000s user + 0.000000s system = 0.180000s CPU (102.6%)
% 
%  num of tris:(3968, 6912), 10880, Octree deep:4
% OctreeIntersect cpu time :  1.549209s wall, 1.550000s user + 0.000000s system = 1.550000s CPU (100.1%)
% normalIntersect cpu time :  1.140628s wall, 1.130000s user + 0.000000s system = 1.130000s CPU (99.1%)
% 
%  num of tris:(16128, 18432), 34560, Octree deep:5
% OctreeIntersect cpu time :  3.878483s wall, 3.870000s user + 0.000000s system = 3.870000s CPU (99.8%)
% normalIntersect cpu time :  4.671456s wall, 4.640000s user + 0.030000s system = 4.670000s CPU (100.0%)
% 
%  num of tris:(65024, 49152), 114176, Octree deep:5
% OctreeIntersect cpu time :  9.649845s wall, 9.650000s user + 0.000000s system = 9.650000s CPU (100.0%)
% normalIntersect cpu time :  22.934571s wall, 22.860000s user + 0.070000s system = 22.930000s CPU (100.0%)
% 
%  num of tris:(261120, 98304), 359424, Octree deep:6
% OctreeIntersect cpu time :  25.118267s wall, 25.120000s user + 0.000000s system = 25.120000s CPU (100.0%)
% normalIntersect cpu time :  129.951804s wall, 129.720000s user + 0.220000s system = 129.940000s CPU (100.0%)
% 
%  num of tris:(408320, 131072), 539392, Octree deep:6
% OctreeIntersect cpu time :  35.477708s wall, 35.480000s user + 0.000000s system = 35.480000s CPU (100.0%)
% normalIntersect cpu time :  306.352480s wall, 306.100000s user + 0.240000s system = 306.340000s CPU (100.0%)