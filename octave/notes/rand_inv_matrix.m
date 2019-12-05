% mat_size = 8;
% while true
%   A = rand(mat_size, mat_size);
%   if rank(A) == mat_size
%     break
%   end
% end
% B = A' * A
% det(B)
% inv(B)

B = [2.0857, 1.9089, 1.6399, 1.7816, 1.6210, 1.3977, 1.6470, 1.6312;
     1.9089, 2.8859, 1.6302, 2.0310, 2.7094, 1.8250, 2.2226, 2.3604;
     1.6399, 1.6302, 1.8399, 1.8891, 1.5613, 1.6317, 1.6831, 1.5128;
     1.7816, 2.0310, 1.8891, 2.1767, 2.0922, 1.8509, 1.8765, 1.7960;
     1.6210, 2.7094, 1.5613, 2.0922, 3.3884, 2.0772, 2.0594, 2.3494;
     1.3977, 1.8250, 1.6317, 1.8509, 2.0772, 2.5621, 2.0007, 1.9702;
     1.6470, 2.2226, 1.6831, 1.8765, 2.0594, 2.0007, 2.9416, 2.1018;
     1.6312, 2.3604, 1.5128, 1.7960, 2.3494, 1.9702, 2.1018, 2.6391;];

round(inv(B) * B)