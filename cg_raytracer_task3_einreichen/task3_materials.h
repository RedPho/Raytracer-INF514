#include "phong.h"
#include "colors.h"

namespace cg{
  /*
  ============================================================================
   Task 3
  ============================================================================
   Adjust the phong materials so that they follow the specification given in
   the task description.
   Look into phong.h for a description what the values in the constructor
   mean. You do not need to define a new material class. Every material
   can be constructed using the Phong material.
   Do NOT rename the materials below!
  */
  Phong phong_redblack{black, 0.0, cyan, 0.0};
  Phong phong_diffuse{white, 0.0, offwhite, 0.0};
  Phong phong_mirror{black, 1.00, white, 0.0};

  //Phong phong_redblack{white, 0.04, offwhite, 50.0};
  //Phong phong_diffuse{white, 0.04, offwhite, 50.0};
  //Phong phong_mirror{white, 0.04, offwhite, 50.0};

  }
//Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, Zeynep Saraoğlu und İpek Semra Süthan