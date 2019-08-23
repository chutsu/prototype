# Linear Algebra


## Trace

\begin{equation}
  \text{tr}(\Mat{A}) = \sum_{i} \Mat{A}_{ii}
\end{equation}


## Rank

The rank $\rho(\Mat{A})$ of a matrix $\Mat{A}$ of $n$ rows and $m$ columns is
defined as \textbf{the number of independent rows or columns}.

- Full rank (non-singular): $\rho(\Mat{A}) = \min(n, m)$
- Not full rank (singular): $\rho(\Mat{A}) < \min(n, m)$


## Condition Number

There are different condition numbers in the following the condition number for
the problem $\Mat{A} \Vec{x} = \Vec{b}$ and matrix inversion are discussed. In
general, the condition number, $\kappa(\cdot)$, for a matrix, $\Mat{A}$, or
computational task such as $\Mat{A} \Vec{x} = \Vec{b}$ measures how sensitive
the output is to perturbations in the input data and to round off errors. If
the condition number is large, even a small error in $\Vec{x}$ would cause a
large error in $\Vec{x}$. On the other hand, if the condition number is small
then the error in $\Vec{x}$ will not be much bigger than the error in
$\Vec{b}$.
%
\begin{align}
  \kappa(\Mat{A}) &\approx 1 \quad \text{well-Conditioned} \\
  \kappa(\Mat{A}) &> 1 \quad \text{ill-Conditioned}
\end{align}

The condition number is defined more precisely to be the maximum ratio of the
relative error in $\Vec{x}$ to the relative error in $\Vec{b}$.

Let $\Vec{e}$ be the error in $\Vec{b}$. Assuming that $\Mat{A}$ is a
nonsingular matrix, the error in the solution $\Mat{A}^{-1} \Vec{b}$ is
$\Mat{A}^{-1} \Vec{e}$. The ratio of the relative error in the solution to the
relative error in $\Vec{b}$ is
\begin{equation}
    \dfrac{\Norm{\Mat{A}^{-1} \Vec{e}}}{\Norm{\Mat{A}^{-1} \Vec{b}}} /
    \dfrac{\Norm{\Vec{e}}}{\Norm{\Vec{b}}}
\end{equation}
which can be rewritten as,
\begin{equation}
  \left(
    \dfrac{\Norm{\Mat{A}^{-1} \Vec{e}}}{\Norm{\Vec{e}}}
  \right)
  \cdot
  \left(
    \dfrac{\Norm{\Vec{b}}}{\Norm{\Mat{A}^{-1} \Vec{b}}}
  \right)
\end{equation}
The maximum value (for nonzero $\Vec{b}$ and $\Vec{e}$) is then seen to be the
product of the two operator norms as follows:
\begin{align}
  % -- LINE 1
  &\max_{\Vec{e}, \Vec{b} \neq 0}
  \left\{
    \left(
      \dfrac{\Norm{\Mat{A}^{-1} \Vec{e}}}{\Norm{\Vec{e}}}
      \cdot
      \dfrac{\Norm{\Vec{b}}}{\Norm{\Mat{A}^{-1} \Vec{b}}}
    \right)
  \right\} \\
  % -- LINE 2
  &= \max_{\Vec{e}, \Vec{b} \neq 0}
  \left\{
    \left(
      \dfrac{\Norm{\Mat{A}^{-1} \Vec{e}}}{\Norm{\Vec{e}}}
    \right)
  \right\}
  \cdot
  \max_{\Vec{e}, \Vec{b} \neq 0}
  \left\{
    \left(
      \dfrac{\Norm{\Vec{b}}}{\Norm{\Mat{A}^{-1} \Vec{b}}}
    \right)
  \right\} \\
  % -- LINE 3
  &= \max_{\Vec{e}, \Vec{b} \neq 0}
  \left\{
    \left(
      \dfrac{\Norm{\Mat{A}^{-1} \Vec{e}}}{\Norm{\Vec{e}}}
    \right)
  \right\}
  \cdot
  \max_{\Vec{e}, \Vec{b} \neq 0}
  \left\{
    \left(
      \dfrac{\Norm{\Mat{A} \Vec{x}}}{\Norm{\Vec{x}}}
    \right)
  \right\} \\
  % -- LINE 4
  &= \Norm{\Mat{A}^{-1}} \cdot \Norm{\Mat{A}}
\end{align}
The same definition is used for any matrix norm, i.e. one that satisfies
\begin{equation}
  \kappa(\Mat{A}) = \Norm{\Mat{A}^{-1}} \cdot \Norm{\Mat{A}}
    \geq \Norm{\Mat{A}^{-1} \cdot \Mat{A}} = 1 .
\end{equation}
When the condition number is exactly one (which can only happen if $\Mat{A}$ is
a scalar multiple of a linear isometry), then a solution algorithm can find (in
principle, meaning if the algorithm introduces no errors of its own) an
approximation of the solution whose precision is no worse than that of the
data.

However, it does not mean that the algorithm will converge rapidly to this
solution, just that it won't diverge arbitrarily because of inaccuracy on the
source data (backward error), provided that the forward error introduced by the
algorithm does not diverge as well because of accumulating intermediate
rounding errors.

The condition number may also be infinite, but this implies that the problem is
ill-posed (does not possess a unique, well-defined solution for each choice of
data -- that is, the matrix is not invertible), and no algorithm can be
expected to reliably find a solution.

The definition of the condition number depends on the choice of norm, as can be
illustrated by two examples.