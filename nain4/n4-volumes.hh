#ifndef N4_VOLUMES_HH
#define N4_VOLUMES_HH

#include "nain4.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4String.hh>
#include <G4SystemOfUnits.hh>
#include <G4Types.hh>
#include <G4Box.hh>
#include <G4VSolid.hh>
#include <G4Sphere.hh>
#include <G4Tubs.hh>

#include <G4VSensitiveDetector.hh>
#include <G4VSolid.hh>
#include <optional>

#define G4D G4double

namespace nain4 {

#define OPT_DOUBLE std::optional<G4double>

#define SENSITIVE(TYPE) TYPE& sensitive(G4VSensitiveDetector* s) { sd = s; return *this; }

enum class BOOL_OP { ADD, SUB, INT };

struct boolean_shape;

struct shape {
  G4LogicalVolume*  volume(G4Material* material) const;
  n4::place          place(G4Material* material) const { return n4::place(volume(material)); }
  shape&             name (G4String    name    ) { name_ = name; return *this; }
  virtual G4VSolid*  solid(                    ) const = 0;
  virtual ~shape() {}

  // boolean operations
  boolean_shape add      (n4::shape& shape);
  boolean_shape subtract (n4::shape& shape);
  boolean_shape intersect(n4::shape& shape);
  boolean_shape add      (G4VSolid*  solid);
  boolean_shape subtract (G4VSolid*  solid);
  boolean_shape intersect(G4VSolid*  solid);

  // Alternative names
  template<class S> boolean_shape join (S shape);
  template<class S> boolean_shape sub  (S shape);
  template<class S> boolean_shape inter(S shape);

protected:
  shape(G4String name) : name_{name} {}
  std::optional<G4VSensitiveDetector*> sd;
  G4String                             name_;
};


struct boolean_shape : shape {
  friend shape;
  G4VSolid* solid() const override;

  boolean_shape& rotate(G4RotationMatrix& rot)    { transformation = HepGeom::Rotate3D{rot}      * transformation; return *this; }
  boolean_shape& at(double x, double y, double z) { transformation = HepGeom::Translate3D{x,y,z} * transformation; return *this; }
  boolean_shape& at(G4ThreeVector    p)           { return at(p.x(), p.y(), p.z()); }
  boolean_shape& name(G4String name)              { name_ = name; return *this; }
private:
  boolean_shape(G4VSolid* a, G4VSolid* b, BOOL_OP op) : shape{a -> GetName()}, a{a}, b{b}, op{op}  {}
  G4VSolid* a;
  G4VSolid* b;
  BOOL_OP   op;
  G4Transform3D transformation = HepGeom::Transform3D::Identity;
};

template<class S> boolean_shape shape::join (S shape){ return add      (shape); }
template<class S> boolean_shape shape::sub  (S shape){ return subtract (shape); }
template<class S> boolean_shape shape::inter(S shape){ return intersect(shape); }


struct box : shape {
  box(G4String name) : shape{name} {}
  box&      x(G4D l) { half_x_ = l / 2; return *this; }
  box&      y(G4D l) { half_y_ = l / 2; return *this; }
  box&      z(G4D l) { half_z_ = l / 2; return *this; }
  box& half_x(G4D l) { half_x_ = l    ; return *this; }
  box& half_y(G4D l) { half_y_ = l    ; return *this; }
  box& half_z(G4D l) { half_z_ = l    ; return *this; }
  box&      cube(G4double l) { return this ->      xyz(l,l,l); }
  box& half_cube(G4double l) { return this -> half_xyz(l,l,l); }
  box&      xyz(G4D x, G4D y, G4D z) { return this ->     x(x).y(y).z(z); }
  box& half_xyz(G4D x, G4D y, G4D z) { return this -> xyz(x*2, y*2, z*2); }
  SENSITIVE(box)
  G4Box* solid() const;
private:
  G4D half_x_;
  G4D half_y_;
  G4D half_z_;
};

struct sphere : shape {
  sphere(G4String name) : shape{name} {}
  sphere& r_inner     (G4D x) { r_inner_     = x; return *this; };
  sphere& r           (G4D x) { r_outer_     = x; return *this; };
  sphere& r_delta     (G4D x) { r_delta_     = x; return *this; };
  sphere& phi_start   (G4D x) { phi_start_   = x; return *this; };
  sphere& phi_end     (G4D x) { phi_end_     = x; return *this; };
  sphere& phi_delta   (G4D x) { phi_delta_   = x; return *this; };
  sphere& theta_start (G4D x) { theta_start_ = x; return *this; };
  sphere& theta_end   (G4D x) { theta_end_   = x; return *this; };
  sphere& theta_delta (G4D x) { theta_delta_ = x; return *this; };
  SENSITIVE(sphere)
  G4VSolid* solid() const;
private:
  OPT_DOUBLE r_inner_;
  OPT_DOUBLE r_delta_;
  OPT_DOUBLE r_outer_;
  G4D   phi_start_   = 0;
  OPT_DOUBLE phi_end_;
  OPT_DOUBLE phi_delta_;
  G4D   theta_start_ = 0;
  OPT_DOUBLE theta_end_;
  OPT_DOUBLE theta_delta_;
  const static constexpr G4D   phi_full = 360 * deg;
  const static constexpr G4D theta_full = 180 * deg;
};

struct tubs : shape {
  tubs(G4String name) : shape{name} {}

  tubs& r_inner  (G4D x) { r_inner_   = x  ; return *this; };
  tubs& r        (G4D x) { r_outer_   = x  ; return *this; };
  tubs& r_delta  (G4D x) { r_delta_   = x  ; return *this; };
  tubs& phi_start(G4D x) { phi_start_ = x  ; return *this; };
  tubs& phi_end  (G4D x) { phi_end_   = x  ; return *this; };
  tubs& phi_delta(G4D x) { phi_delta_ = x  ; return *this; };
  tubs& half_z   (G4D x) { half_z_    = x  ; return *this; };
  tubs& z        (G4D x) { half_z_    = x/2; return *this; };
  SENSITIVE(tubs)
  G4Tubs* solid() const;
private:
  OPT_DOUBLE r_inner_;
  OPT_DOUBLE r_delta_;
  OPT_DOUBLE r_outer_;
  G4D   phi_start_ = 0;
  OPT_DOUBLE phi_end_;
  OPT_DOUBLE phi_delta_;
  G4D   half_z_;
  const static constexpr G4D phi_full = 360 * deg;
};


#undef OPT_DOUBLE
#undef G4D

}; // namespace nain4

namespace n4 { using namespace nain4; }

#endif // N4_VOLUMES_HH
