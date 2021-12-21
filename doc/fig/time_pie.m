clc 
clear all 
close all
x = [2112, 34560, 114176, 359424, 539392];
y1 = [0.025488  0.378303 0.000041  0.012697] ./  0.416649;
y2 = [0.486104  4.264657 0.000307 0.033481 ] ./ 4.784649;
y3 = [4.089319 21.790178 0.000555 0.085220 ] ./ 25.965357;
label = ["edgeCal" 'initOctree' 'pruneTree' 'calTest'];
em = ["", "", "", ""];
pie(y1,em);
legend(label);
figure;
pie(y2,em);
legend(label);
figure;
pie(y3,em);
legend(label);

%  num of tris:(960, 1152), 2112, Octree deep:3
% edgeCal cpu time :  0.025488s wall, 0.030000s user + 0.000000s system = 0.030000s CPU (117.7%)
% initOctree cpu time :  0.378303s wall, 0.370000s user + 0.000000s system = 0.370000s CPU (97.8%)
% pruneTree cpu time :  0.000041s wall, 0.000000s user + 0.000000s system = 0.000000s CPU (n/a%)
% calTest cpu time :  0.012697s wall, 0.020000s user + 0.000000s system = 0.020000s CPU (157.5%)
% total cpu time :  0.416649s wall, 0.420000s user + 0.000000s system = 0.420000s CPU (100.8%)
% 
%  num of tris:(16128, 18432), 34560, Octree deep:5
% edgeCal cpu time :  0.486104s wall, 0.490000s user + 0.000000s system = 0.490000s CPU (100.8%)
% initOctree cpu time :  4.264657s wall, 4.260000s user + 0.010000s system = 4.270000s CPU (100.1%)
% pruneTree cpu time :  0.000307s wall, 0.000000s user + 0.000000s system = 0.000000s CPU (n/a%)
% calTest cpu time :  0.033481s wall, 0.030000s user + 0.000000s system = 0.030000s CPU (89.6%)
% total cpu time :  4.784649s wall, 4.780000s user + 0.010000s system = 4.790000s CPU (100.1%)
% 
%  num of tris:(65024, 49152), 114176, Octree deep:5
% edgeCal cpu time :  1.225118s wall, 1.220000s user + 0.000000s system = 1.220000s CPU (99.6%)
% initOctree cpu time :  8.692308s wall, 8.690000s user + 0.010000s system = 8.700000s CPU (100.1%)
% pruneTree cpu time :  0.000260s wall, 0.000000s user + 0.000000s system = 0.000000s CPU (n/a%)
% calTest cpu time :  0.046626s wall, 0.050000s user + 0.000000s system = 0.050000s CPU (107.2%)
% total cpu time :  9.964390s wall, 9.960000s user + 0.010000s system = 9.970000s CPU (100.1%)
% 
%  num of tris:(261120, 98304), 359424, Octree deep:6
% edgeCal cpu time :  4.089319s wall, 4.070000s user + 0.030000s system = 4.100000s CPU (100.3%)
% initOctree cpu time :  21.790178s wall, 21.780000s user + 0.000000s system = 21.780000s CPU (100.0%)
% pruneTree cpu time :  0.000555s wall, 0.000000s user + 0.000000s system = 0.000000s CPU (n/a%)
% calTest cpu time :  0.085220s wall, 0.090000s user + 0.000000s system = 0.090000s CPU (105.6%)
% total cpu time :  25.965357s wall, 25.940000s user + 0.030000s system = 25.970000s CPU (100.0%)
% 
%  num of tris:(408320, 131072), 539392, Octree deep:6
% edgeCal cpu time :  6.302988s wall, 6.280000s user + 0.020000s system = 6.300000s CPU (100.0%)
% initOctree cpu time :  29.977963s wall, 29.970000s user + 0.000000s system = 29.970000s CPU (100.0%)
% pruneTree cpu time :  0.000555s wall, 0.000000s user + 0.000000s system = 0.000000s CPU (n/a%)
% calTest cpu time :  0.108709s wall, 0.110000s user + 0.000000s system = 0.110000s CPU (101.2%)
% total cpu time :  36.390304s wall, 36.360000s user + 0.020000s system = 36.380000s CPU (100.0%)