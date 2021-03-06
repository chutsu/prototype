Equi-distant Distortion
=======================

.. math::

  \begin{align}
    r &= \sqrt{x^{2} + y^{2}} \\
    \theta &= \arctan{(r)} \\
    \theta_d &= \theta (1 + k_1 \theta^2 + k_2 \theta^4
      + k_3 \theta^6 + k_4 \theta^8) \\
    x' &= (\theta_d / r) \cdot x \\
    y' &= (\theta_d / r) \cdot y
  \end{align}



Equi-distant Point Jacobian
---------------------------

.. math::

  \begin{align}
    \dfrac{\partial{\Vec{x}'}}{\partial{\Vec{x}}} &=
      \begin{bmatrix}
        J_{11} & J_{12} \\
        J_{21} & J_{22}
      \end{bmatrix} \\ \\
      %
      r &= \sqrt{x^{2} + y^{2}} \\
      \theta &= \arctan(r) \\
      \theta_d &= \theta (1 + k_1 \theta^2 + k_2 \theta^4
        + k_3 \theta^6 + k_4 \theta^8) \\
      \theta_d' &= 1 + 3 k_1 \theta^2 + 5 k_2 \theta^4
        + 7 k_3 \theta^6 + 9 k_4 \theta^8 \\
      \theta_r &= 1 / (r^2 + 1) \\ \\
      %
      s &= \theta_d / r \\
      s_r &= \theta_d' \theta_r / r - \theta_d / r^2 \\ \\
      %
      r_x &= 1 / r x \\
      r_y &= 1 / r y \\ \\
      %
      J_{11} &= s + x s_r r_x \\
      J_{12} &= x s_r r_y \\
      J_{21} &= y s_r r_x \\
      J_{22} &= s + y s_r r_y
  \end{align}



Equi-distant Parameter Jacobian
-------------------------------

.. math::

  \begin{align}
    \dfrac{\partial{\Vec{x}'}}{\partial{\Vec{d}}} &=
      \begin{bmatrix}
        J_{11} & J_{12} & J_{13} & J_{14} \\
        J_{21} & J_{22} & J_{23} & J_{24}
      \end{bmatrix} \\ \\
    r &= \sqrt{x^{2} + y^{2}} \\
    \theta &= \arctan(r) \\ \\
    J_{11} &= x \theta^3 / r \\
    J_{12} &= x \theta^5 / r \\
    J_{13} &= x \theta^7 / r \\
    J_{14} &= x \theta^9 / r \\ \\
    J_{21} &= y \theta^3 / r \\
    J_{22} &= y \theta^5 / r \\
    J_{23} &= y \theta^7 / r \\
    J_{24} &= y \theta^9 / r
  \end{align}
