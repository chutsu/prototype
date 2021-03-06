#!/usr/bin/octave -qf
graphics_toolkit("fltk");

% Parser command line args
arg_list = argv();
pos_csv = arg_list{1};
pos_prop_csv = arg_list{2};
att_csv = arg_list{3};
att_prop_csv = arg_list{4};
accel_csv = arg_list{5};
gyro_csv = arg_list{6};

% Parse data
pos_data = csvread(pos_csv, 0, 0);
pos_prop_data = csvread(pos_prop_csv, 0, 0);
att_data = csvread(att_csv, 0, 0);
att_prop_data = csvread(att_prop_csv, 0, 0);
accel_data = csvread(accel_csv, 0, 0);
gyro_data = csvread(gyro_csv, 0, 0);

function euler = quat2euler(q)
  qw = q(1);
  qx = q(2);
  qy = q(3);
  qz = q(4);

  qw2 = qw**2;
  qx2 = qx**2;
  qy2 = qy**2;
  qz2 = qz**2;

  t1 = atan2(2 * (qx * qw + qz * qy), (qw2 - qx2 - qy2 + qz2));
  t2 = asin(2 * (qy * qw - qx * qz));
  t3 = atan2(2 * (qx * qy + qz * qw), (qw2 + qx2 - qy2 - qz2));
  euler = [t1; t2; t3];
endfunction

% Plot figure
figure(1);

subplot(221);
hold on;
t = pos_data(:, 1) * 1e-9;
plot(t, pos_data(:, 2), "r.", "markersize", 20);
plot(t, pos_data(:, 3), "b.", "markersize", 20);
plot(t, pos_data(:, 4), "g.", "markersize", 20);
legend("x", "y", "z");
xlabel("Time [s]");
ylabel("Position [m]");
xlim([0, max(t)]);
title("Position");

subplot(222);
hold on;
t = pos_prop_data(:, 1) * 1e-9;
plot(t, pos_prop_data(:, 2), "r.", "markersize", 20);
plot(t, pos_prop_data(:, 3), "b.", "markersize", 20);
plot(t, pos_prop_data(:, 4), "g.", "markersize", 20);
legend("x", "y", "z");
xlabel("Time [s]");
ylabel("Position [m]");
xlim([0, max(t)]);
title("Propagated Position");

subplot(223);
hold on;
t = att_data(:, 1) * 1e-9;

rpy_data = zeros(rows(att_data), 3);
for i = 1:rows(att_data)
  rpy = quat2euler(att_data(i, 2:end));
  assert(columns(rpy) == 1);
  rpy_data(i, :) = rpy';
endfor

plot(t, rpy_data(:, 1), "r.", "markersize", 20);
plot(t, rpy_data(:, 2), "b.", "markersize", 20);
plot(t, rpy_data(:, 3), "g.", "markersize", 20);
legend("x", "y", "z");
xlabel("Time [s]");
ylabel("Attitude [rad]");
xlim([0, max(t)]);
ylim([-1.0, 1.0]);
title("Attitude");

subplot(224);
hold on;
t = att_prop_data(:, 1) * 1e-9;

rpy_data = zeros(rows(att_prop_data), 3);
for i = 1:rows(att_prop_data)
  rpy = quat2euler(att_prop_data(i, 2:end));
  assert(columns(rpy) == 1);
  rpy_data(i, :) = rpy';
endfor

plot(t, rpy_data(:, 1), "r.", "markersize", 20);
plot(t, rpy_data(:, 2), "b.", "markersize", 20);
plot(t, rpy_data(:, 3), "g.", "markersize", 20);
legend("x", "y", "z");
xlabel("Time [s]");
ylabel("Attitude [rad]");
xlim([0, max(t)]);
ylim([-1.0, 1.0]);
title("Propagated Attitude");

figure(2);
subplot(211);
hold on;
t = accel_data(:, 1) * 1e-9;
plot(t, accel_data(:, 2), "r.", "markersize", 20);
plot(t, accel_data(:, 3), "b.", "markersize", 20);
plot(t, accel_data(:, 4), "g.", "markersize", 20);
legend("x", "y", "z");
xlabel("Time [s]");
ylabel("Acceleration [ms^-2]");
xlim([0, max(t)]);
title("Simulated Accelerometer Measurements");

subplot(212);
hold on;
t = gyro_data(:, 1) * 1e-9;
plot(t, gyro_data(:, 2), "r.", "markersize", 20);
plot(t, gyro_data(:, 3), "b.", "markersize", 20);
plot(t, gyro_data(:, 4), "g.", "markersize", 20);
legend("x", "y", "z");
xlabel("Time [s]");
ylabel("Angular Velocity [rad s^-1]");
xlim([0, max(t)]);
title("Simulated Gyroscope Measurements");

ginput()
