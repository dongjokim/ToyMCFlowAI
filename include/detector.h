 enum DETECTOR{
          D_TPC, //TPC full coverage
          D_V0A,
          D_V0C,
          D_COUNT
  };
static double decAcc[D_COUNT][2] = {
        {-0.8,0.8},
        {2.8,5.1},   // V0A
        {-3.7,-1.7} // V0C
};
static const char *pdetn[4] = {"TPC","VZEROA","VZEROC","VZERO"};
