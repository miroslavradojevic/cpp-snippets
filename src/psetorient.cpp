#include <iostream>
#include <fstream>
#include <cmath>
#ifdef MAX
#undef MAX
#endif
#define MAX(a, b) ((a)>(b)?(a):(b))

using namespace std;
#define n 3

double absd(double val){ if(val>0){ return val;} else { return -val;} }
double hypot2(double x, double y) { return sqrt(x*x+y*y); }
void tred2(double V[n][n], double d[n], double e[n]) {
    /*  This is derived from the Algol procedures tred2 by */
    /*  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for */
    /*  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding */
    /*  Fortran subroutine in EISPACK. */
        int i, j, k;
        double scale;
        double f, g, h;
        double hh;
        for (j = 0; j < n; j++) {d[j] = V[n-1][j]; }

        /* Householder reduction to tridiagonal form. */

        for (i = n-1; i > 0; i--) {
            /* Scale to avoid under/overflow. */
            scale = 0.0;
            h = 0.0;
            for (k = 0; k < i; k++) { scale = scale + fabs(d[k]); }
            if (scale == 0.0) {
                e[i] = d[i-1];
                for (j = 0; j < i; j++) { d[j] = V[i-1][j]; V[i][j] = 0.0;  V[j][i] = 0.0; }
            } else {

                /* Generate Householder vector. */

                for (k = 0; k < i; k++) { d[k] /= scale; h += d[k] * d[k]; }
                f = d[i-1];
                g = sqrt(h);
                if (f > 0) { g = -g; }
                e[i] = scale * g;
                h = h - f * g;
                d[i-1] = f - g;
                for (j = 0; j < i; j++) { e[j] = 0.0; }

                /* Apply similarity transformation to remaining columns. */

                for (j = 0; j < i; j++) {
                    f = d[j];
                    V[j][i] = f;
                    g = e[j] + V[j][j] * f;
                    for (k = j+1; k <= i-1; k++) { g += V[k][j] * d[k]; e[k] += V[k][j] * f; }
                    e[j] = g;
                }
                f = 0.0;
                for (j = 0; j < i; j++) { e[j] /= h; f += e[j] * d[j]; }
                hh = f / (h + h);
                for (j = 0; j < i; j++) { e[j] -= hh * d[j]; }
                for (j = 0; j < i; j++) {
                    f = d[j]; g = e[j];
                    for (k = j; k <= i-1; k++) { V[k][j] -= (f * e[k] + g * d[k]); }
                    d[j] = V[i-1][j];
                    V[i][j] = 0.0;
                }
            }
            d[i] = h;
        }

        /* Accumulate transformations. */

        for (i = 0; i < n-1; i++) {
            V[n-1][i] = V[i][i];
            V[i][i] = 1.0;
            h = d[i+1];
            if (h != 0.0) {
                for (k = 0; k <= i; k++) { d[k] = V[k][i+1] / h;}
                for (j = 0; j <= i; j++) {
                    g = 0.0;
                    for (k = 0; k <= i; k++) { g += V[k][i+1] * V[k][j]; }
                    for (k = 0; k <= i; k++) { V[k][j] -= g * d[k]; }
                }
            }
            for (k = 0; k <= i; k++) { V[k][i+1] = 0.0;}
        }
        for (j = 0; j < n; j++) { d[j] = V[n-1][j]; V[n-1][j] = 0.0; }
        V[n-1][n-1] = 1.0;
        e[0] = 0.0;
}

void tql2(double V[n][n], double d[n], double e[n]) {
/*  Symmetric tridiagonal QL algorithm. */
/*  This is derived from the Algol procedures tql2, by */
/*  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for */
/*  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding */
/*  Fortran subroutine in EISPACK. */

    int i, j, k, l, m;
    double f;
    double tst1;
    double eps;
    int iter;
    double g, p, r;
    double dl1, h, c, c2, c3, el1, s, s2;

    for (i = 1; i < n; i++) { e[i-1] = e[i]; }
    e[n-1] = 0.0;

    f = 0.0;
    tst1 = 0.0;
    eps = pow(2.0, -52.0);
    for (l = 0; l < n; l++) {

        /* Find small subdiagonal element */

        tst1 = MAX(tst1, fabs(d[l]) + fabs(e[l]));
        m = l;
        while (m < n) {
            if (fabs(e[m]) <= eps*tst1) { break; }
            m++;
        }

        /* If m == l, d[l] is an eigenvalue, */
        /* otherwise, iterate. */

        if (m > l) {
            iter = 0;
            do {
                iter = iter + 1;  /* (Could check iteration count here.) */
                /* Compute implicit shift */
                g = d[l];
                p = (d[l+1] - g) / (2.0 * e[l]);
                r = hypot2(p, 1.0);
                if (p < 0) { r = -r; }
                d[l] = e[l] / (p + r);
                d[l+1] = e[l] * (p + r);
                dl1 = d[l+1];
                h = g - d[l];
                for (i = l+2; i < n; i++) { d[i] -= h; }
                f = f + h;
                /* Implicit QL transformation. */
                p = d[m]; c = 1.0; c2 = c; c3 = c;
                el1 = e[l+1]; s = 0.0; s2 = 0.0;
                for (i = m-1; i >= l; i--) {
                    c3 = c2;
                    c2 = c;
                    s2 = s;
                    g = c * e[i];
                    h = c * p;
                    r = hypot2(p, e[i]);
                    e[i+1] = s * r;
                    s = e[i] / r;
                    c = p / r;
                    p = c * d[i] - s * g;
                    d[i+1] = h + s * (c * g + s * d[i]);
                    /* Accumulate transformation. */
                    for (k = 0; k < n; k++) {
                        h = V[k][i+1];
                        V[k][i+1] = s * V[k][i] + c * h;
                        V[k][i] = c * V[k][i] - s * h;
                    }
                }
                p = -s * s2 * c3 * el1 * e[l] / dl1;
                e[l] = s * p;
                d[l] = c * p;

                /* Check for convergence. */
            } while (fabs(e[l]) > eps*tst1);
        }
        d[l] = d[l] + f;
        e[l] = 0.0;
    }

    /* Sort eigenvalues and corresponding vectors. */
    for (i = 0; i < n-1; i++) {
        k = i;
        p = d[i];
        for (j = i+1; j < n; j++) {
            if (d[j] < p) {
                k = j;
                p = d[j];
            }
        }
        if (k != i) {
            d[k] = d[i];
            d[i] = p;
            for (j = 0; j < n; j++) {
                p = V[j][i];
                V[j][i] = V[j][k];
                V[j][k] = p;
            }
        }
    }
}

void eigen_decomposition(double A[n][n], double V[n][n], double d[n]) {

    double e[n];
    double da[3];
    double dt, dat;
    double vet[3];
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
                V[i][j] = A[i][j];
        }
    }

    tred2(V, d, e);
    tql2(V, d, e);

    /* Sort the eigen values and vectors by abs eigen value */
    da[0]=absd(d[0]); da[1]=absd(d[1]); da[2]=absd(d[2]);

    if((da[0]>=da[1])&&(da[0]>da[2])) {
        dt=d[2];   dat=da[2];    vet[0]=V[0][2];    vet[1]=V[1][2];    vet[2]=V[2][2];
        d[2]=d[0]; da[2]=da[0];  V[0][2] = V[0][0]; V[1][2] = V[1][0]; V[2][2] = V[2][0];
        d[0]=dt;   da[0]=dat;    V[0][0] = vet[0];  V[1][0] = vet[1];  V[2][0] = vet[2];
    }
    else if((da[1]>=da[0])&&(da[1]>da[2])) {
        dt=d[2];   dat=da[2];    vet[0]=V[0][2];    vet[1]=V[1][2];    vet[2]=V[2][2];
        d[2]=d[1]; da[2]=da[1];  V[0][2] = V[0][1]; V[1][2] = V[1][1]; V[2][2] = V[2][1];
        d[1]=dt;   da[1]=dat;    V[0][1] = vet[0];  V[1][1] = vet[1];  V[2][1] = vet[2];
    }

    if(da[0]>da[1]) {
        dt=d[1];   dat=da[1];    vet[0]=V[0][1];    vet[1]=V[1][1];    vet[2]=V[2][1];
        d[1]=d[0]; da[1]=da[0];  V[0][1] = V[0][0]; V[1][1] = V[1][0]; V[2][1] = V[2][0];
        d[0]=dt;   da[0]=dat;    V[0][0] = vet[0];  V[1][0] = vet[1];  V[2][0] = vet[2];
    }

}

int main() { // int argc, char *argv[]

        // generate set of points and estimate the direction and centroid
        float t1[134][3] =  {{352.008, 205.211, 34.230}, {354.062, 203.947, 34.149}, {352.008, 205.211, 34.230}, {300.895, 225.513, 36.960}, {303.652, 224.845, 36.838},{306.040, 223.834, 36.277}, {308.107, 222.792, 35.889}, {310.522, 221.700, 35.853}, {315.174, 221.073, 35.840}, {318.413, 219.519, 35.894}, {320.778, 218.167, 35.571}, {323.737, 216.530, 35.392}, {326.965, 214.136, 34.872}, {329.983, 212.680, 34.677}, {332.785, 212.192, 34.797}, {335.388, 212.065, 35.034}, {338.085, 212.029, 35.288}, {341.196, 211.597, 35.215}, {343.751, 210.367, 34.893}, {346.716, 208.683, 34.864}, {349.176, 207.155, 34.576}, {352.008, 205.211, 34.230}, {356.326, 202.700, 34.309}, {354.062, 203.947, 34.149}, {356.326, 202.700, 34.309}, {314.598, 220.987, 35.768}, {317.027, 219.982, 35.942}, {314.598, 220.987, 35.768}, {319.775, 218.820, 35.943}, {317.027, 219.982, 35.942}, {319.775, 218.820, 35.943}, {363.496, 197.085, 33.801}, {361.594, 199.988, 33.786}, {358.873, 201.346, 33.983}, {356.821, 202.978, 34.215}, {355.631, 203.922, 34.188}, {354.483, 204.137, 34.084}, {352.069, 205.021, 34.262}, {348.509, 207.226, 34.585}, {346.441, 208.685, 34.775}, {343.945, 209.968, 34.955}, {340.647, 211.237, 35.089}, {338.285, 211.648, 34.720}, {335.680, 211.525, 34.856}, {333.411, 211.756, 34.949}, {329.881, 213.279, 34.970}, {326.166, 215.285, 35.562}, {324.065, 216.337, 35.994}, {322.230, 217.472, 36.011}, {319.775, 218.820, 35.943}, {315.432, 220.890, 36.062}, {319.065, 219.035, 35.913}, {315.432, 220.890, 36.062}, {321.621, 217.797, 35.813}, {319.065, 219.035, 35.913}, {321.621, 217.797, 35.813}, {362.536, 197.735, 33.337}, {361.657, 199.338, 33.349}, {360.162, 200.765, 33.499}, {357.929, 202.563, 33.888}, {355.973, 203.546, 34.123}, {354.412, 204.623, 34.168}, {353.933, 204.119, 34.399}, {352.697, 204.575, 34.457}, {350.098, 205.981, 34.508}, {345.204, 209.239, 35.375}, {341.447, 211.299, 35.377}, {338.684, 211.429, 35.224}, {336.005, 212.001, 34.801}, {332.641, 212.592, 34.912}, {328.921, 213.678, 35.257}, {325.950, 215.435, 35.880}, {324.029, 216.544, 35.766}, {322.613, 217.342, 35.690}, {321.621, 217.797, 35.813}, {355.225, 204.368, 34.769}, {356.718, 202.317, 34.513}, {355.225, 204.368, 34.769}, {322.020, 218.478, 35.595}, {324.102, 218.161, 35.637}, {325.577, 216.658, 35.623}, {326.965, 214.991, 35.366}, {328.326, 213.346, 34.648}, {329.997, 212.662, 34.343}, {331.772, 212.309, 34.384}, {333.679, 212.391, 34.625}, {335.482, 212.156, 35.068}, {337.741, 211.809, 35.182}, {340.266, 211.026, 35.097}, {342.788, 210.278, 35.005}, {344.533, 209.561, 34.541}, {346.414, 208.976, 34.186}, {348.393, 207.643, 34.532}, {350.140, 207.043, 34.415}, {352.095, 206.150, 34.269}, {353.511, 205.194, 34.391}, {355.225, 204.368, 34.769}, {359.139, 201.086, 34.284}, {356.718, 202.317, 34.513}, {359.139, 201.086, 34.284}, {345.925, 209.622, 35.117}, {347.967, 207.973, 34.762}, {345.925, 209.622, 35.117}, {299.968, 225.801, 36.862}, {302.635, 224.785, 36.787}, {305.462, 224.349, 36.760}, {307.590, 223.374, 36.872}, {309.922, 222.321, 36.766}, {311.815, 221.569, 36.244}, {314.334, 220.873, 36.142}, {316.637, 220.202, 35.860}, {318.806, 219.774, 35.608}, {320.732, 218.523, 35.430}, {323.172, 217.234, 35.489}, {325.728, 215.719, 35.030}, {328.661, 213.026, 33.840}, {330.520, 212.325, 34.113}, {332.747, 212.307, 34.343}, {335.837, 211.533, 35.089}, {338.947, 211.948, 35.027}, {343.254, 211.282, 35.189}, {345.925, 209.622, 35.117}, {350.307, 206.155, 34.588}, {347.966, 207.973, 34.762}, {350.307, 206.155, 34.588}, {343.718, 210.243, 34.359}, {345.646, 209.198, 34.720}, {348.435, 207.548, 34.342}, {352.491, 206.650, 33.879}, {355.874, 204.551, 33.446}, {358.324, 202.249, 33.310}, {359.799, 199.432, 32.832}, {362.279, 196.747, 33.117}, {365.798, 194.059, 32.997}};

        float t2[92][3] = {{352.008, 205.211, 34.23},{354.062, 203.947, 34.149},{352.008, 205.211, 34.23},{320.778, 218.167, 35.571},{323.737, 216.53, 35.392},{326.965, 214.136, 34.872},{329.983, 212.68, 34.677},{332.785, 212.192, 34.797},{335.388, 212.065, 35.034},{338.085, 212.029, 35.288},{341.196, 211.597, 35.215},{343.751, 210.367, 34.893},{346.716, 208.683, 34.864},{349.176, 207.155, 34.576},{352.008, 205.211, 34.23},{354.062, 203.947, 34.149},{319.775, 218.82, 35.943},{319.775, 218.82, 35.943},{354.483, 204.137, 34.084},{352.069, 205.021, 34.262},{348.509, 207.226, 34.585},{346.441, 208.685, 34.775},{343.945, 209.968, 34.955},{340.647, 211.237, 35.089},{338.285, 211.648, 34.72},{335.68, 211.525, 34.856},{333.411, 211.756, 34.949},{329.881, 213.279, 34.97},{326.166, 215.285, 35.562},{324.065, 216.337, 35.994},{322.23, 217.472, 36.011},{319.775, 218.82, 35.943},{319.065, 219.035, 35.913},{321.621, 217.797, 35.813},{319.065, 219.035, 35.913},{321.621, 217.797, 35.813},{354.412, 204.623, 34.168},{353.933, 204.119, 34.399},{352.697, 204.575, 34.457},{350.098, 205.981, 34.508},{345.204, 209.239, 35.375},{341.447, 211.299, 35.377},{338.684, 211.429, 35.224},{336.005, 212.001, 34.801},{332.641, 212.592, 34.912},{328.921, 213.678, 35.257},{325.95, 215.435, 35.88},{324.029, 216.544, 35.766},{322.613, 217.342, 35.69},{321.621, 217.797, 35.813},{355.225, 204.368, 34.769},{355.225, 204.368, 34.769},{322.02, 218.478, 35.595},{324.102, 218.161, 35.637},{325.577, 216.658, 35.623},{326.965, 214.991, 35.366},{328.326, 213.346, 34.648},{329.997, 212.662, 34.343},{331.772, 212.309, 34.384},{333.679, 212.391, 34.625},{335.482, 212.156, 35.068},{337.741, 211.809, 35.182},{340.266, 211.026, 35.097},{342.788, 210.278, 35.005},{344.533, 209.561, 34.541},{346.414, 208.976, 34.186},{348.393, 207.643, 34.532},{350.14, 207.043, 34.415},{352.095, 206.15, 34.269},{353.511, 205.194, 34.391},{355.225, 204.368, 34.769},{345.925, 209.622, 35.117},{347.967, 207.973, 34.762},{345.925, 209.622, 35.117},{318.806, 219.774, 35.608},{320.732, 218.523, 35.43},{323.172, 217.234, 35.489},{325.728, 215.719, 35.03},{328.661, 213.026, 33.84},{330.52, 212.325, 34.113},{332.747, 212.307, 34.343},{335.837, 211.533, 35.089},{338.947, 211.948, 35.027},{343.254, 211.282, 35.189},{345.925, 209.622, 35.117},{350.307, 206.155, 34.588},{347.966, 207.973, 34.762},{350.307, 206.155, 34.588},{343.718, 210.243, 34.359},{345.646, 209.198, 34.72},{348.435, 207.548, 34.342},{352.491, 206.65, 33.879}};
        for (int i = 0; i < 92; ++i) t2[i][0] += 20;

        float t3[40][3] =  {{329.983, 212.68, 34.677},{332.785, 212.192, 34.797},{335.388, 212.065, 35.034},{338.085, 212.029, 35.288},{341.196, 211.597, 35.215},{343.751, 210.367, 34.893},{346.441, 208.685, 34.775},{343.945, 209.968, 34.955},{340.647, 211.237, 35.089},{338.285, 211.648, 34.72},{335.68, 211.525, 34.856},{333.411, 211.756, 34.949},{329.881, 213.279, 34.97},{345.204, 209.239, 35.375},{341.447, 211.299, 35.377},{338.684, 211.429, 35.224},{336.005, 212.001, 34.801},{332.641, 212.592, 34.912},{328.921, 213.678, 35.257},{328.326, 213.346, 34.648},{329.997, 212.662, 34.343},{331.772, 212.309, 34.384},{333.679, 212.391, 34.625},{335.482, 212.156, 35.068},{337.741, 211.809, 35.182},{340.266, 211.026, 35.097},{342.788, 210.278, 35.005},{344.533, 209.561, 34.541},{346.414, 208.976, 34.186},{345.925, 209.622, 35.117},{345.925, 209.622, 35.117},{328.661, 213.026, 33.84},{330.52, 212.325, 34.113},{332.747, 212.307, 34.343},{335.837, 211.533, 35.089},{338.947, 211.948, 35.027},{343.254, 211.282, 35.189},{345.925, 209.622, 35.117},{343.718, 210.243, 34.359},{345.646, 209.198, 34.72}};
        for (int i = 0; i < 40; ++i) t3[i][0] += 40;

        float t4[19][3] = {{332.785, 212.192, 34.797},{335.388, 212.065, 35.034},{338.085, 212.029, 35.288},{341.196, 211.597, 35.215},{340.647, 211.237, 35.089},{338.285, 211.648, 34.72},{335.68, 211.525, 34.856},{333.411, 211.756, 34.949},{341.447, 211.299, 35.377},{338.684, 211.429, 35.224},{336.005, 212.001, 34.801},{332.641, 212.592, 34.912},{333.679, 212.391, 34.625},{335.482, 212.156, 35.068},{337.741, 211.809, 35.182},{340.266, 211.026, 35.097},{332.747, 212.307, 34.343},{335.837, 211.533, 35.089},{338.947, 211.948, 35.027}};
        for (int i = 0; i < 19; ++i) t4[i][0] += 60;

        int nr_nbrs1 = 134;
        int nr_nbrs2 = 92;
        int nr_nbrs3 = 40;
        int nr_nbrs4 = 19;

        float mu1[3] = {0,0,0};
        float mu2[3] = {0,0,0};
        float mu3[3] = {0,0,0};
        float mu4[3] = {0,0,0};

        double cov1[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
        double cov2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
        double cov3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
        double cov4[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

        double  vec1[3][3];
        double  vec2[3][3];
        double  vec3[3][3];
        double  vec4[3][3];

        double  eig[3];

        for (int i = 0; i < nr_nbrs1; ++i) {mu1[0] += t1[i][0]; mu1[1] += t1[i][1]; mu1[2] += t1[i][2];}
        for (int i = 0; i < 3; ++i)  mu1[i] /= nr_nbrs1;

        for (int i = 0; i < nr_nbrs2; ++i) {mu2[0] += t2[i][0]; mu2[1] += t2[i][1]; mu2[2] += t2[i][2];}
        for (int i = 0; i < 3; ++i)  mu2[i] /= nr_nbrs2;

        for (int i = 0; i < nr_nbrs3; ++i) {mu3[0] += t3[i][0]; mu3[1] += t3[i][1]; mu3[2] += t3[i][2];}
        for (int i = 0; i < 3; ++i)  mu3[i] /= nr_nbrs3;

        for (int i = 0; i < nr_nbrs4; ++i) {mu4[0] += t4[i][0]; mu4[1] += t4[i][1]; mu4[2] += t4[i][2];}
        for (int i = 0; i < 3; ++i)  mu4[i] /= nr_nbrs4;

        for (int i = 0; i < nr_nbrs1; ++i) {
            float dx = t1[i][0]-mu1[0];
            float dy = t1[i][1]-mu1[1];
            float dz = t1[i][2]-mu1[2];
            cov1[0][0] += dx*dx;cov1[0][1] += dx*dy;cov1[0][2] += dx*dz;
            cov1[1][0] += dy*dx;cov1[1][1] += dy*dy;cov1[1][2] += dy*dz;
            cov1[2][0] += dz*dx;cov1[2][1] += dz*dy;cov1[2][2] += dz*dz;
        }

        for (int i = 0; i < nr_nbrs2; ++i) {
            float dx = t2[i][0]-mu2[0];
            float dy = t2[i][1]-mu2[1];
            float dz = t2[i][2]-mu2[2];
            cov2[0][0] += dx*dx;cov2[0][1] += dx*dy;cov2[0][2] += dx*dz;
            cov2[1][0] += dy*dx;cov2[1][1] += dy*dy;cov2[1][2] += dy*dz;
            cov2[2][0] += dz*dx;cov2[2][1] += dz*dy;cov2[2][2] += dz*dz;
        }

        for (int i = 0; i < nr_nbrs3; ++i) {
            float dx = t3[i][0]-mu3[0];
            float dy = t3[i][1]-mu3[1];
            float dz = t3[i][2]-mu3[2];
            cov3[0][0] += dx*dx;cov3[0][1] += dx*dy;cov3[0][2] += dx*dz;
            cov3[1][0] += dy*dx;cov3[1][1] += dy*dy;cov3[1][2] += dy*dz;
            cov3[2][0] += dz*dx;cov3[2][1] += dz*dy;cov3[2][2] += dz*dz;
        }

        for (int i = 0; i < nr_nbrs4; ++i) {
            float dx = t4[i][0]-mu4[0];
            float dy = t4[i][1]-mu4[1];
            float dz = t4[i][2]-mu4[2];
            cov4[0][0] += dx*dx;cov4[0][1] += dx*dy;cov4[0][2] += dx*dz;
            cov4[1][0] += dy*dx;cov4[1][1] += dy*dy;cov4[1][2] += dy*dz;
            cov4[2][0] += dz*dx;cov4[2][1] += dz*dy;cov4[2][2] += dz*dz;
        }

        for (int i = 0; i < 3; ++i) for (int j = 0; j < 3; ++j) {
            cov1[i][j] /= nr_nbrs1;
            cov2[i][j] /= nr_nbrs2;
            cov3[i][j] /= nr_nbrs3;
            cov4[i][j] /= nr_nbrs4;
        }

        eigen_decomposition(cov1, vec1, eig);
        eigen_decomposition(cov2, vec2, eig);
        eigen_decomposition(cov3, vec3, eig);
        eigen_decomposition(cov4, vec4, eig);

        // export to swc
        string sp = "psetorient.swc";
        ofstream f;
        f.open(sp.c_str(), ofstream::out | ofstream::trunc);

        // data
        int cnt = 1;
        for (int i = 0; i < nr_nbrs1; ++i) f<<(cnt++)<<" "<<1<<" "<<t1[i][0]<<" "<<t1[i][1]<<" "<<t1[i][2]<<" "<<1<<" "<<(-1)<<endl;
        for (int i = 0; i < nr_nbrs2; ++i) f<<(cnt++)<<" "<<1<<" "<<t2[i][0]<<" "<<t2[i][1]<<" "<<t2[i][2]<<" "<<1<<" "<<(-1)<<endl;
        for (int i = 0; i < nr_nbrs3; ++i) f<<(cnt++)<<" "<<1<<" "<<t3[i][0]<<" "<<t3[i][1]<<" "<<t3[i][2]<<" "<<1<<" "<<(-1)<<endl;
        for (int i = 0; i < nr_nbrs4; ++i) f<<(cnt++)<<" "<<1<<" "<<t4[i][0]<<" "<<t4[i][1]<<" "<<t4[i][2]<<" "<<1<<" "<<(-1)<<endl;

        // direction
        f<<(cnt++)<<" "<<3<<" "<<mu1[0]<<" "<<mu1[1]<<" "<<mu1[2]<<" "<<0.1<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<3<<" "<<(mu1[0]+50*vec1[0][2])<<" "<<(mu1[1]+50*vec1[1][2])<<" "<<(mu1[2]+50*vec1[2][2])<<" "<<0.1<<" "<<(cnt-2)<<endl;

        f<<(cnt++)<<" "<<3<<" "<<mu2[0]<<" "<<mu2[1]<<" "<<mu2[2]<<" "<<0.1<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<3<<" "<<(mu2[0]+30*vec2[0][2])<<" "<<(mu2[1]+30*vec2[1][2])<<" "<<(mu2[2]+30*vec2[2][2])<<" "<<0.1<<" "<<(cnt-2)<<endl;

        f<<(cnt++)<<" "<<3<<" "<<mu3[0]<<" "<<mu3[1]<<" "<<mu3[2]<<" "<<0.1<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<3<<" "<<(mu3[0]+30*vec3[0][2])<<" "<<(mu3[1]+30*vec3[1][2])<<" "<<(mu3[2]+30*vec3[2][2])<<" "<<0.1<<" "<<(cnt-2)<<endl;

        f<<(cnt++)<<" "<<3<<" "<<mu4[0]<<" "<<mu4[1]<<" "<<mu4[2]<<" "<<0.1<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<3<<" "<<(mu4[0]+30*vec4[0][2])<<" "<<(mu4[1]+30*vec4[1][2])<<" "<<(mu4[2]+30*vec4[2][2])<<" "<<0.1<<" "<<(cnt-2)<<endl;

        // centroid
        f<<(cnt++)<<" "<<2<<" "<<mu1[0]<<" "<<mu1[1]<<" "<<mu1[2]<<" "<<2<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<2<<" "<<mu2[0]<<" "<<mu2[1]<<" "<<mu2[2]<<" "<<2<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<2<<" "<<mu3[0]<<" "<<mu3[1]<<" "<<mu3[2]<<" "<<2<<" "<<(-1)<<endl;
        f<<(cnt++)<<" "<<2<<" "<<mu4[0]<<" "<<mu4[1]<<" "<<mu4[2]<<" "<<2<<" "<<(-1)<<endl;

        f.close();
        cout << " exported:\t" << sp << endl;

    return 0;
}