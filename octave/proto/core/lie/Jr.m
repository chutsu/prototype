function J = Jr(theta)
  theta_norm = norm(theta);
  theta_norm_sq = theta_norm * theta_norm;
  theta_norm_cube = theta_norm_sq * theta_norm;
  theta_skew = skew(theta);
  theta_skew_sq = theta_skew * theta_skew;

  J = eye(3);
  J -= ((1 - cos(theta_norm)) / theta_norm_sq) * theta_skew;
  J += (theta_norm - sin(theta_norm)) / (theta_norm_cube) * theta_skew_sq;
endfunction
