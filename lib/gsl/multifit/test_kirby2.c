#define kirby2_N       151
#define kirby2_P       5

#define kirby2_NTRIES  1

/* double kirby2_x0[kirby2_P] = { 2, -0.1, 0.003, -0.001, 0.00001 }; */

static double kirby2_x0[kirby2_P] = { 1.5, -0.15, 0.0025, -0.0015, 0.00002 }; 

static double kirby2_epsrel = 1.0e-5;

static double kirby2_sigma[kirby2_P] = {
  8.7989634338E-02,
  4.1182041386E-03,
  4.1856520458E-05,
  5.8931897355E-05,
  2.0129761919E-07
};

static double kirby2_F1[kirby2_N] = {
       0.0082E0,
       0.0112E0,
       0.0149E0,
       0.0198E0,
       0.0248E0,
       0.0324E0,
       0.0420E0,
       0.0549E0,
       0.0719E0,
       0.0963E0,
       0.1291E0,
       0.1710E0,
       0.2314E0,
       0.3227E0,
       0.4809E0,
       0.7084E0,
       1.0220E0,
       1.4580E0,
       1.9520E0,
       2.5410E0,
       3.2230E0,
       3.9990E0,
       4.8520E0,
       5.7320E0,
       6.7270E0,
       7.8350E0,
       9.0250E0,
      10.2670E0,
      11.5780E0,
      12.9440E0,
      14.3770E0,
      15.8560E0,
      17.3310E0,
      18.8850E0,
      20.5750E0,
      22.3200E0,
      22.3030E0,
      23.4600E0,
      24.0600E0,
      25.2720E0,
      25.8530E0,
      27.1100E0,
      27.6580E0,
      28.9240E0,
      29.5110E0,
      30.7100E0,
      31.3500E0,
      32.5200E0,
      33.2300E0,
      34.3300E0,
      35.0600E0,
      36.1700E0,
      36.8400E0,
      38.0100E0,
      38.6700E0,
      39.8700E0,
      40.0300E0,
      40.5000E0,
      41.3700E0,
      41.6700E0,
      42.3100E0,
      42.7300E0,
      43.4600E0,
      44.1400E0,
      44.5500E0,
      45.2200E0,
      45.9200E0,
      46.3000E0,
      47.0000E0,
      47.6800E0,
      48.0600E0,
      48.7400E0,
      49.4100E0,
      49.7600E0,
      50.4300E0,
      51.1100E0,
      51.5000E0,
      52.1200E0,
      52.7600E0,
      53.1800E0,
      53.7800E0,
      54.4600E0,
      54.8300E0,
      55.4000E0,
      56.4300E0,
      57.0300E0,
      58.0000E0,
      58.6100E0,
      59.5800E0,
      60.1100E0,
      61.1000E0,
      61.6500E0,
      62.5900E0,
      63.1200E0,
      64.0300E0,
      64.6200E0,
      65.4900E0,
      66.0300E0,
      66.8900E0,
      67.4200E0,
      68.2300E0,
      68.7700E0,
      69.5900E0,
      70.1100E0,
      70.8600E0,
      71.4300E0,
      72.1600E0,
      72.7000E0,
      73.4000E0,
      73.9300E0,
      74.6000E0,
      75.1600E0,
      75.8200E0,
      76.3400E0,
      76.9800E0,
      77.4800E0,
      78.0800E0,
      78.6000E0,
      79.1700E0,
      79.6200E0,
      79.8800E0,
      80.1900E0,
      80.6600E0,
      81.2200E0,
      81.6600E0,
      82.1600E0,
      82.5900E0,
      83.1400E0,
      83.5000E0,
      84.0000E0,
      84.4000E0,
      84.8900E0,
      85.2600E0,
      85.7400E0,
      86.0700E0,
      86.5400E0,
      86.8900E0,
      87.3200E0,
      87.6500E0,
      88.1000E0,
      88.4300E0,
      88.8300E0,
      89.1200E0,
      89.5400E0,
      89.8500E0,
      90.2500E0,
      90.5500E0,
      90.9300E0,
      91.2000E0,
      91.5500E0,
      92.2000E0
};


static double kirby2_F0[kirby2_N] = {
    9.65E0,
   10.74E0,
   11.81E0,
   12.88E0,
   14.06E0,
   15.28E0,
   16.63E0,
   18.19E0,
   19.88E0,
   21.84E0,
   24.00E0,
   26.25E0,
   28.86E0,
   31.85E0,
   35.79E0,
   40.18E0,
   44.74E0,
   49.53E0,
   53.94E0,
   58.29E0,
   62.63E0,
   67.03E0,
   71.25E0,
   75.22E0,
   79.33E0,
   83.56E0,
   87.75E0,
   91.93E0,
   96.10E0,
  100.28E0,
  104.46E0,
  108.66E0,
  112.71E0,
  116.88E0,
  121.33E0,
  125.79E0,
  125.79E0,
  128.74E0,
  130.27E0,
  133.33E0,
  134.79E0,
  137.93E0,
  139.33E0,
  142.46E0,
  143.90E0,
  146.91E0,
  148.51E0,
  151.41E0,
  153.17E0,
  155.97E0,
  157.76E0,
  160.56E0,
  162.30E0,
  165.21E0,
  166.90E0,
  169.92E0,
  170.32E0,
  171.54E0,
  173.79E0,
  174.57E0,
  176.25E0,
  177.34E0,
  179.19E0,
  181.02E0,
  182.08E0,
  183.88E0,
  185.75E0,
  186.80E0,
  188.63E0,
  190.45E0,
  191.48E0,
  193.35E0,
  195.22E0,
  196.23E0,
  198.05E0,
  199.97E0,
  201.06E0,
  202.83E0,
  204.69E0,
  205.86E0,
  207.58E0,
  209.50E0,
  210.65E0,
  212.33E0,
  215.43E0,
  217.16E0,
  220.21E0,
  221.98E0,
  225.06E0,
  226.79E0,
  229.92E0,
  231.69E0,
  234.77E0,
  236.60E0,
  239.63E0,
  241.50E0,
  244.48E0,
  246.40E0,
  249.35E0,
  251.32E0,
  254.22E0,
  256.24E0,
  259.11E0,
  261.18E0,
  264.02E0,
  266.13E0,
  268.94E0,
  271.09E0,
  273.87E0,
  276.08E0,
  278.83E0,
  281.08E0,
  283.81E0,
  286.11E0,
  288.81E0,
  291.08E0,
  293.75E0,
  295.99E0,
  298.64E0,
  300.84E0,
  302.02E0,
  303.48E0,
  305.65E0,
  308.27E0,
  310.41E0,
  313.01E0,
  315.12E0,
  317.71E0,
  319.79E0,
  322.36E0,
  324.42E0,
  326.98E0,
  329.01E0,
  331.56E0,
  333.56E0,
  336.10E0,
  338.08E0,
  340.60E0,
  342.57E0,
  345.08E0,
  347.02E0,
  349.52E0,
  351.44E0,
  353.93E0,
  355.83E0,
  358.32E0,
  360.20E0,
  362.67E0,
  364.53E0,
  367.00E0,
  371.30E0
};

static void
kirby2_checksol(const double x[], const double sumsq,
                const double epsrel, const char *sname,
                const char *pname)
{
  size_t i;
  const double sumsq_exact = 3.9050739624E+00;
  const double kirby2_x[kirby2_P] = {
    1.6745063063E+00, -1.3927397867E-01, 2.5961181191E-03,
   -1.7241811870E-03, 2.1664802578E-05 };

  gsl_test_rel(sumsq, sumsq_exact, epsrel, "%s/%s sumsq",
               sname, pname);

  for (i = 0; i < kirby2_P; ++i)
    {
      gsl_test_rel(x[i], kirby2_x[i], epsrel, "%s/%s i=%zu",
                   sname, pname, i);
    }
}

static int
kirby2_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  double b[kirby2_P];
  size_t i;

  for (i = 0; i < kirby2_P; i++)
    {
      b[i] = gsl_vector_get(x, i);
    }

  for (i = 0; i < kirby2_N; i++)
    {
      double x = kirby2_F0[i];
      double y = ((b[0] + x* (b[1]  + x * b[2]))
                  / (1 + x*(b[3]  + x *b[4])));
      gsl_vector_set (f, i, kirby2_F1[i] - y);
    }

  return GSL_SUCCESS;
}

static int
kirby2_df (const gsl_vector * x, void *params, gsl_matrix * df)
{
  double b[kirby2_P];
  size_t i;

  for (i = 0; i < kirby2_P; i++)
    {
      b[i] = gsl_vector_get(x, i);
    }

  for (i = 0; i < kirby2_N; i++)
    {
      double x = kirby2_F0[i];
      double u = (b[0] + x*(b[1] + x*b[2]));
      double v = (1 + x*(b[3] + x*b[4]));
      gsl_matrix_set (df, i, 0, -1/v);
      gsl_matrix_set (df, i, 1, -x/v);
      gsl_matrix_set (df, i, 2, -x*x/v);
      gsl_matrix_set (df, i, 3, x*u/(v*v));
      gsl_matrix_set (df, i, 4, x*x*u/(v*v));
    }

  return GSL_SUCCESS;
}

static gsl_multifit_function_fdf kirby2_func =
{
  &kirby2_f,
  &kirby2_df,
  NULL,
  kirby2_N,
  kirby2_P,
  NULL,
  0,
  0
};

static test_fdf_problem kirby2_problem =
{
  "nist-kirby2",
  kirby2_x0,
  kirby2_sigma,
  &kirby2_epsrel,
  kirby2_NTRIES,
  &kirby2_checksol,
  &kirby2_func
};
