Inertial Measurement Unit (IMU)
===============================

IMU kinematics
--------------

\begin{align}
  % Position
  \DotPos{\world}{\sensor} &= \Vel{\world}{\sensor} \\
  % Orientation
  \DotQuat{\world}{\sensor} &=
    \dfrac{1}{2} \mathbf{\Omega}
    (\imuGyroMeas, \noiseGyro, \biasGyro)
    \Quat{\world}{\sensor} \\
  % Velocity
  \DotVel{\world}{\sensor} &=
    \Rot{\world}{\sensor}
    (\imuAccelMeas + \noiseAccel - \biasAccel) + \gravity \\
  % Gyro Bias
  \dot{\biasGyro} &= \noise_{\biasGyro} \\
  % Accel Bias
  \dot{\biasAccel} &= - \dfrac{1}{\tau} \biasAccel + \noise_{\biasGyro}
\end{align}

The matrix $\mathbf{\Omega}$ is formed from the estimated angular rate
$\imuGyro = \imuGyroMeas + \noiseGyro - \biasGyro$

\begin{equation}
  \mathbf{F}_{c} = \begin{bmatrix}
    % Row 1
    \Zeros{3}{3}
    & \Zeros{3}{3}
    & \I_{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3} \\
    % Row 2
    \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Rot{\world}{\sensor}
    & \Zeros{3}{3} \\
    % Row 3
    \Zeros{3}{3}
    & \Skew{\Rot{\world}{\sensor}(\imuAccelMeas - \biasAccel)}
    & \Zeros{3}{3}
    & \Zeros{3}{3}
    & -\Rot{\world}{\sensor} \\
    % Row 4
    \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3} \\
    % Row 5
    \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3}
    & \Zeros{3}{3}
    & -\dfrac{1}{\tau} \ones_{3}
  \end{bmatrix}
\end{equation}