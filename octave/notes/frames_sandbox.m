graphics_toolkit("fltk");
addpath(genpath("proto"));

% T_WS = [
% -0.328389, 0.047525, -0.943346, 0.859962;
% 0.047525, 0.998300, 0.033750, 0.400259;
% 0.943346, -0.033750, -0.330089, 0.488291;
% 0.000000, 0.000000, 0.000000, 1.000000;
% ]
%
% T_WF = [
%   -0.110489, -0.238704, 0.964786, 0.000000;
%   0.993877, -0.025686, 0.107466, 0.000000;
%   -0.000871, 0.970753, 0.240081, 0.000000;
%   0.000000, 0.000000, 0.000000, 1.000000;
% ]

% T_WS = [
%   0.996288, 0.047368, -0.071873, 0.102476,
%   0.047368, 0.395487, 0.917250, -0.149503,
%   0.071873, -0.917250, 0.391775, -0.013268,
%   0.000000, 0.000000, 0.000000, 1.000000
% ]
%
% T_WF = [
%   0.978500, -0.054377, 0.198951, 0.000000,
%   0.205888, 0.200568, -0.957801, 0.000000,
%   0.012179, 0.978170, 0.207451, 0.000000,
%   0.000000, 0.000000, 0.000000, 1.000000
% ]

% C_WS = [
%   -0.324045, 0.036747, -0.945328;
%   0.036747, 0.998980, 0.026236;
%   0.945328, -0.026236, -0.325065;
% ];
%
% T_SC0 = [
% 	0.000000, -1.000000, 0.000000, 0.000000
% 	1.000000, 0.000000, 0.000000, 0.000000
% 	0.000000, 0.000000, 1.000000, 0.000000
% 	0.000000, 0.000000, 0.000000, 1.000000
% ];
%
% T_C0F = [
%   0.986976, -0.069762, 0.144956, -0.426290;
%   -0.082567, -0.993015, 0.084277, 0.196928;
%   0.138064, -0.095148, -0.985842, 0.959312;
%   0.000000, 0.000000, 0.000000, 1.000000
% ];
%
% r_C0F = tf_trans(T_C0F);
% C_C0F = tf_rot(T_C0F);
%
% T_WS = tf(C_WS, zeros(3, 1));
% T_WF = T_WS * T_SC0 * T_C0F
%
% % Calculate offset
% offset = tf_trans(T_WF) * -1
%
% % Set T_WF as origin and adjust T_WS accordingly
% T_WF = tf(tf_rot(T_WF), zeros(3, 1))
% T_WS = tf(tf_rot(T_WS), offset)


T_WF = [
  0.999929, 0.007898, 0.008976, 1.277089
  -0.007829, 0.999940, -0.007698, 0.973154
  -0.009036, 0.007627, 0.999930, 0.000947
  0.000000, 0.000000, 0.000000, 1.000000
];

T_WM = [
  0.172378, 0.984005, -0.044956, 1.368825
  -0.980332, 0.166924, -0.105291, 1.921483
  -0.096103, 0.062222, 0.993425, 1.132677
  0.000000, 0.000000, 0.000000, 1.000000
];

T_MC0 = [
  0.080016, -0.909654, 0.407588, 0.096363
  -0.996769, -0.070128, 0.039169, 0.024375
  -0.007047, -0.409405, -0.912326, -0.077259
  0.000000, 0.000000, 0.000000, 1.000000
];


% Plot
figure(1);
hold on;
grid on;
view(3);
% draw_frame(T_WS, 0.1);
% draw_frame(T_WF, 0.1);

draw_frame(T_WF, 0.1);
draw_frame(T_WM, 0.1);
draw_frame(T_WM * T_MC0, 0.1);

xlabel("x");
ylabel("y");
zlabel("z");
axis('equal');
ginput();
