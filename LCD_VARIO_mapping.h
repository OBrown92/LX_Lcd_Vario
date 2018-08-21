const uint8_t IND[][3] = {
  {0, 38, 8}, //I01
  {0, 38, 2},
  {0, 38, 4},
  {1, 10, 4},
  {1, 10, 2}, //I05
  {1, 10, 8},
  {1, 10, 128},
  {1, 10, 32},
  {1, 10, 64},
  {1, 8, 4}, //I10
  {1, 8, 2},
  {1, 8, 8},
  {1, 8, 128},
  {1, 8, 32},
  {1, 8, 64}, //I15
  {1, 6, 4}, 
  {1, 6, 2},
  {1, 6, 8},
  {1, 6, 128},
  {1, 6, 32}, //I20
  {1, 6, 64},
  {1, 4, 4}, 
  {1, 4, 2},
  {1, 4, 8},
  {1, 4, 128}, //I25
  {1, 4, 32},
  {1, 4, 64},
  {1, 2, 4}, 
  {1, 2, 2},
  {1, 2, 8}, //I30
  {1, 2, 128},
  {1, 2, 32},
  {1, 2, 64},
  {1, 0, 4},
  {1, 0, 2}, //I35
  {1, 0, 8},
  {0, 0, 128},
  {0, 0, 32},
  {0, 0, 64},
  {0, 0, 4}, //I40
  {0, 0, 2},
  {0, 0, 8},
  {0, 2, 128},
  {0, 2, 32},
  {0, 2, 64}, //I45
  {0, 2, 4},
  {0, 2, 2},
  {0, 2, 8},
  {0, 4, 128},
  {0, 4, 32}, //50
  {0, 4, 64}
};

const uint8_t SCR[][3] = {
  {0, 38, 1},
  {1, 10, 1},
  {1, 10, 16},
  {1, 8, 16},
  {1, 6, 16},
  {1, 4, 1},
  {1, 6, 1},
  {1, 2, 1},
  {1, 2, 16},
  {0, 0, 1},
  {0, 0, 1},
  {0, 2, 16},
  {0, 4, 16}
};

const uint8_t SYM[][3] = {
  {1, 4, 16}, //DIS
  {0, 12, 1}, //BAT
  {0, 28, 16}, //BUG
  {0, 14, 1}, //ALT
  {0, 16, 16}, //GP_
  {0, 18, 1}, //VAR
  {0, 20, 1}, //REL
  {0, 24, 16}, //NET
  {0, 22, 16}, //SC_
  {0, 26, 2}, //N__
  {0, 26, 4}, //M__
  {0, 26, 1}, //L__
  {0, 8, 1}, //KM_
  {0, 8, 16}, //FT_
  {0, 34, 1}, //2._
  {0, 36, 16}, //5__
  {0, 36, 32}, //10_
  {0, 36, 2}, //20_
  {0, 36, 64}, //m/s
  {0, 38, 128}, //kts
  {0, 38, 32}, //VRM
  {0, 36, 8} //MCM
};