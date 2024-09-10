#ifndef CG_BOX_H
#define CG_BOX_H
//==============================================================================
#include "renderable.h"
#include "vec.h"
//==============================================================================
namespace cg {
//==============================================================================
/// Implicit function of a cube.
class Box : public Renderable {
  //============================================================================
 
  public:
  //============================================================================
   Box(const Material& m);
   //---------------------------------------------------------------------------
   std::optional<Intersection> check_intersection(
       const Ray& r, double min_t = 0) const override;
   //---------------------------------------------------------------------------
   /// needed for polymorphic copy
  make_clonable(Renderable, Box);
};
//==============================================================================
}  // namespace cg
//==============================================================================
#endif
