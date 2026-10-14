---
title: Numerical methods
---

## Notations on time series

For a time serie $x^n$ with sample period $dt$, the following notations are used for forward averaging and differentiating operators:

$$
    \mu_{t+} x^n = \frac{x^{n+1} + x^n}{2}, \delta_{t+} x^n = \frac{x^{n+1} - x^n}{dt}.
$$

For vectorial $x$ and $y$, scalar products and norms are written as:

$$
\begin{align*}
    (x^n, y^n)&= (x^n)^\intercal y^n,\\\
    \vert\vert x^n\vert\vert^2 &= (x^n, x^n).
\end{align*}
$$

In prevision of power-balance computations, weighted norms (with weigth matrix $A$) are noted as
$$
    \vert\vert x^n\vert\vert_A^2 = (x^n)^\intercal A x^n
$$

<details><summary>Useful identities</summary>

Operator composition:

$$
\begin{align}
    \delta_{t+} \delta_{t+} x^n &= \frac{1}{dt^2} \left(x^{n+2} - 2x^{n+1} + x^n\right) \\\
    \mu_{t+} \mu_{t+} x^n &= \frac{1}{4} \left(x^{n+2} + 2 x^{n+1} + x^n\right) = \frac{dt^2}{4} (\delta_{t+} \delta_{t+} x^n) + x^{n+1}\\\
    \mu_{t+} \delta_{t+} x^n &= \delta_{t+} \mu_{t+} x^n = \frac{1}{2dt}(x^{n+2} - x^n )
\end{align}
$$

Scalar products and norms:

$$
\begin{align}
    \vert\vert \delta_{t+} x^n \vert\vert^2 &= \frac{1}{dt^2} \left(\vert\vert x^{n+1} \vert\vert^2 - 2 (x^{n+1}, x^n) + \vert\vert x^{n} \vert\vert^2  \right) \\\
    \vert\vert \mu_{t+} x^n \vert\vert^2 &= \frac{1}{4} \left(\vert\vert x^{n+1} \vert\vert^2 + 2 (x^{n+1}, x^n) + \vert\vert x^{n} \vert\vert^2  \right)\\\
    (\mu_{t+}x^n, \delta_{t+} x^n) &= \frac{1}{2dt} \left(\vert\vert x^{n+1} \vert\vert^2 - \vert\vert x^{n} \vert\vert^2 \right) = \frac{1}{2} \delta_{t+} \left(\vert\vert x^{n} \vert\vert^2 \right)
\end{align}
$$

</details>

## Störmer-Velert (or Leapfrog) scheme

The leapfrog time stepping scheme is used extensively as the basis of more complex numerical methods in this library. In its original form, is applies to Hamiltonian systems with no dissipations or input forcing. It is second order accurate in time (the error scales with $\mathcal O (dt^2)$) and symplectic. It does not preserve a direct discrete equivalent to the continuous time energy of the system, but a modified discrete energy preserved by the scheme may be written as a serie perturbation of the original energy. In the nonlinear case, the serie is in general infinite, Fortunately, in the linear case, it greatly simplifies. For these linear systems positivity of the modified energy depends both on the spectral radius of the continuous time ODE of interest and on the sample period. Ensuring positivity results in the simuilation stability. This condition usually yields a inequality of the same form than the CFL condition. 

The following section present progressive extensions of the scheme to systems with input forcing and/or dissipations.

### Isolated, conservative case

First consider the simplest case of a standard Hamiltonian system with state $\alpha=[p^\intercal, q^\intercal]^\intercal$ (we also consider here the mass matrix $M$ and stiffness matrix $K$ to be symmetric for simplicity):

$$
\underbrace{
\begin{bmatrix}
    \partial_t p\\
    \partial_t q
\end{bmatrix}}_{f = \partial_t \alpha}
=
\underbrace{
\begin{bmatrix}
    0 & - \mathbb I\\
    \mathbb I & 0
\end{bmatrix}}_J
\underbrace{
\begin{bmatrix}
    M^{-1} p \\
    K q
\end{bmatrix}}_{e = \nabla H}
.
$$

The associated Hamiltonian writes

$$
    H(p, q) = \frac{1}{2} \left(\vert\vert p\vert\vert_{M^{-1}}^2 + \vert\vert q\vert\vert_{K}^2 \right),
$$ 

and is preserved by the continuous time trajectory owing to the swew-symmetry of the interconnection matrix J: 

$$
    \frac{d}{dt} H = \nabla H^\intercal \partial_t \alpha = e^\intercal f = f^\intercal J^\intercal f = 0
$$


The leapfrog scheme consists of discretizing the $p$ and $q$ variables on interleaved time grids. Then, the scheme writes

$$
\begin{align*}
    \delta_{t+} p^n = - K q^{n+1/2}, \\\
    \delta_{t+} q^{n-1/2} = M^{-1} p^{n}.
\end{align*}
$$

<details><summary>Intermediate power computations</summary>
Before writing the power balance, the second equation can be "recentered" on the main time grid by applying it the forward averaging operator, yielding:

$$
\delta_{t+} \mu_{t+} q^{n-1/2} = M^{-1} \mu_{t+} p^{n}.
$$

The discrete power balance is then obtained by left-multiplying both update equations by the associated averaged efforts

$$
\begin{align*}
    &(M^{-1} \mu_{t+} p^n, \delta_{t+} p^n) = -(M^{-1} \mu_{t+} p^n, K q^{n+1/2}), \\\
    \Leftrightarrow &\frac{1}{2} \delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} = -(M^{-1} \mu_{t+} p^n, K q^{n+1/2}),
\end{align*}
$$

and 

$$
\begin{align*}
    &(K \mu_{t+}\mu_{t+} q^{n-1/2},  \delta_{t+} \mu_{t+} q^{n-1/2}) = (K \mu_{t+}\mu_{t+} q^{n-1/2},  M^{-1} \mu_{t+} p^{n}), \\\
    \Leftrightarrow &\frac{1}{2} \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} = (K \mu_{t+}\mu_{t+} q^{n-1/2},  M^{-1} \mu_{t+} p^{n}).
\end{align*}
$$

Summing both results yields (using identities on operators)

$$
\begin{align*}
    &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = (M^{-1} \mu_{t+} p^n, K (\mu_{t+}\mu_{t+} q^{n-1/2} - q^{n+1/2})), \\\
    \Leftrightarrow &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = \frac{dt^2}{4}(M^{-1} \mu_{t+} p^n, K \delta_{t+} \delta_{t+} q^{n-1/2}), \\\
    \Leftrightarrow &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = \frac{dt^2}{4}(M^{-1} \mu_{t+} p^n, K M^{-1} \delta_{t+} p^n), \\\
    \Leftrightarrow &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{\widetilde M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = 0,
\end{align*}
$$

where in the last line, the weigth matrix for the discrete kinetic energy has been modified to

$$
\widetilde M^{-1} = M^{-1} \left(1 - \frac{dt^2}{4} K M^{-1}\right).
$$



</details>

The power balance for this scheme writes

$$
    \delta_{t+} (E_k^n + E_p^n) = 0,
$$

with 

$$
    E_k^n = \frac{1}{2} \vert\vert p^n \vert\vert^2_{\widetilde M^{-1}}, \quad E_p^n = \frac{1}{2} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K},
$$

and modified mass matrix 

$$
\widetilde M^{-1} = M^{-1} \left(1 - \frac{dt^2}{4} K M^{-1}\right).
$$

Positivity of the discrete energy is a sufficient condition for the simulation stability. Positivity follows from the spectral radius of $\frac{dt^2}{4} K M^{-1}$ to be less than 1. This is equivalent to the CFL condition in the case of wave equations.

### Isolated, dissipative case

Dissipations are added to the previous systems by introducing dissipation matrices $R_P \geq 0$ and $R_q \geq 0$:

$$
\underbrace{
\begin{bmatrix}
    \partial_t p\\
    \partial_t q
\end{bmatrix}}_{f = \partial_t \alpha}
=
\underbrace{
\begin{bmatrix}
    -R_p & - \mathbb I\\
    \mathbb I & -R_q
\end{bmatrix}}_J
\underbrace{
\begin{bmatrix}
    M^{-1} p \\
    K q
\end{bmatrix}}_{e = \nabla H}
.
$$

The continuous time Hamiltonian stays the same than in the conservative case, but now decreases with time owing to dissipations, as

$$
    \frac{d}{dt} H = e^t f = - \vert\vert M^{-1} p \vert\vert^2_{R_p} - \vert\vert K p \vert\vert^2_{R_q} \leq 0.
$$

A modified scheme, including the dissipation terms evaluated in a centered manner writes

$$
\begin{align*}
    \delta_{t+} p^n = - R_p M^{-1} \mu_{t+} p^n - K q^{n+1/2}, \\\
    \delta_{t+} q^{n-1/2} = - R_q K \mu_{t+} q^{n-1/2} +  M^{-1} p^{n}.
\end{align*}
$$

<details><summary>Intermediate power computations</summary>

The procedure is similar than in the conservative case. The second equation recentered writes

$$
\delta_{t+} \mu_{t+} q^{n-1/2} = - R_q K \mu_{t+} \mu_{t+} q^{n-1/2} + M^{-1} \mu_{t+} p^{n}.
$$

The discrete power balance is then obtained by left-multiplying both update equations by the associated averaged efforts

$$
\begin{align*}
    &(M^{-1} \mu_{t+} p^n, \delta_{t+} p^n) = -(M^{-1} \mu_{t+} p^n, R_p M^{-1} \mu_{t+} p^n + K q^{n+1/2}), \\\
    \Leftrightarrow &\frac{1}{2} \delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} = - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p} -(M^{-1} \mu_{t+} p^n, K q^{n+1/2}),
\end{align*}
$$

and 

$$
\begin{align*}
    &(K \mu_{t+}\mu_{t+} q^{n-1/2},  \delta_{t+} \mu_{t+} q^{n-1/2}) = (K \mu_{t+}\mu_{t+} q^{n-1/2},  - R_q K \mu_{t+} \mu_{t+} q^{n-1/2} + M^{-1} \mu_{t+} p^{n}), \\\
    \Leftrightarrow &\frac{1}{2} \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} =
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}
    + (K \mu_{t+}\mu_{t+} q^{n-1/2},  M^{-1} \mu_{t+} p^{n}).
\end{align*}
$$

Summing both results yields (using identities on operators)

$$
\begin{align*}
    &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = (M^{-1} \mu_{t+} p^n, K (\mu_{t+}\mu_{t+} q^{n-1/2} - q^{n+1/2})) 
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}, \\\
    \Leftrightarrow &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = \frac{dt^2}{4}(M^{-1} \mu_{t+} p^n, K \delta_{t+} \delta_{t+} q^{n-1/2})
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}, \\\
    \Leftrightarrow &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = \frac{dt^2}{4}(M^{-1} \mu_{t+} p^n, - K R_q K \delta_{t+} \mu_{t+} q^{n-1/2} + K M^{-1} \delta_{t+} p^n )
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}, \\\
    \Leftrightarrow &\frac{1}{2} \left(\delta_{t+} \vert\vert p^n \vert\vert^2_{\widetilde M^{-1}} + \delta_{t+} \vert\vert \mu_{t+} q^{n-1/2} \vert\vert^2_{K} \right) = 
    - \frac{dt^2}{4}(M^{-1} \mu_{t+} p^n, K R_q K \delta_{t+} \mu_{t+} q^{n-1/2})
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}, \\\
    \Leftrightarrow &\delta_{t+} E_k^n+ \delta_{t+} E_p^n = 
    - \frac{dt^2}{4}(\delta_{t+} \mu_{t+} q^{n-1/2} + R_q K \mu_{t+} \mu_{t+} q^{n-1/2}, K R_q K \delta_{t+} \mu_{t+} q^{n-1/2})
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q},\\\
    \Leftrightarrow &\delta_{t+} E_k^n+ \delta_{t+} E_p^n + \frac{dt^2}{8} \delta_{t+} \vert \vert \mu_{t+} q^{n-1/2}\vert \vert^2_{K R_q K R_q K}= 
    - \frac{dt^2}{4}(\delta_{t+} \mu_{t+} q^{n-1/2}, K R_q K \delta_{t+} \mu_{t+} q^{n-1/2})
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}, \\\
    \Leftrightarrow &\delta_{t+} E_k^n+ \delta_{t+} E_p^n + \frac{dt^2}{8} \delta_{t+} \vert \vert \mu_{t+} q^{n-1/2}\vert \vert^2_{K R_q K R_q K}= 
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \vert\vert K \mu_{t+}\mu_{t+} q^{n-1/2} \vert\vert^2_{R_q}
    - \frac{dt^2}{4}\vert\vert K \delta_{t+} \mu_{t+} q^{n-1/2}\vert\vert^2_{R_q},\\\
    \Leftrightarrow &\delta_{t+} E_k^n+ \delta_{t+} E_p^n + \frac{dt^2}{8} \delta_{t+} \vert \vert \mu_{t+} q^{n-1/2}\vert \vert^2_{K R_q K R_q K}= 
    - \vert\vert M^{-1} \mu_{t+} p^n \vert\vert^2_{R_p}
    - \mu_{t+} \vert \vert K \mu_{t+} q^{n-1/2} \vert \vert^2_{R_q},
\end{align*}
$$

where the weigth matrix for the discrete kinetic energy has been modified to

$$
\widetilde M^{-1} = M^{-1} \left(1 - \frac{dt^2}{4} K M^{-1}\right).
$$



</details>