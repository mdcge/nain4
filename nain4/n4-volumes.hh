#ifndef N4_VOLUMES_HH
#define N4_VOLUMES_HH

#include "nain4.hh"

#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4String.hh>
#include <G4SystemOfUnits.hh>
#include <G4Types.hh>
#include <G4Box.hh>
#include <G4Sphere.hh>
#include <G4Tubs.hh>

#include <optional>

#define LOGICAL inline G4LogicalVolume* logical(G4Material* material) const \
  { return n4::logical(solid(), material); }

#define PLACE inline n4::place place(G4Material* material) const       \
  { return n4::place(logical(material)); }

#define G4D G4double

namespace nain4 {

#define OPT_DOUBLE std::optional<G4double>

struct box {
  box(G4String name) : name{name} {}
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
  G4Box* solid() const;
  LOGICAL
  PLACE
private:
  G4String name;
  G4D half_x_;
  G4D half_y_;
  G4D half_z_;
};

struct sphere {
  sphere(G4String name) : name{name} {}
  sphere& r_inner     (G4D x) { r_inner_     = x; return *this; };
  sphere& r           (G4D x) { r_outer_     = x; return *this; };
  sphere& r_delta     (G4D x) { r_delta_     = x; return *this; };
  sphere& phi_start   (G4D x) { phi_start_   = x; return *this; };
  sphere& phi_end     (G4D x) { phi_end_     = x; return *this; };
  sphere& phi_delta   (G4D x) { phi_delta_   = x; return *this; };
  sphere& theta_start (G4D x) { theta_start_ = x; return *this; };
  sphere& theta_end   (G4D x) { theta_end_   = x; return *this; };
  sphere& theta_delta (G4D x) { theta_delta_ = x; return *this; };
  G4Sphere* solid() const;
  LOGICAL
  PLACE
private:
  G4String name;
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

struct tubs {
  tubs(G4String name) : name{name} {}

  tubs& r_inner  (G4D x) { r_inner_   = x  ; return *this; };
  tubs& r        (G4D x) { r_outer_   = x  ; return *this; };
  tubs& r_delta  (G4D x) { r_delta_   = x  ; return *this; };
  tubs& phi_start(G4D x) { phi_start_ = x  ; return *this; };
  tubs& phi_end  (G4D x) { phi_end_   = x  ; return *this; };
  tubs& phi_delta(G4D x) { phi_delta_ = x  ; return *this; };
  tubs& half_z   (G4D x) { half_z_    = x  ; return *this; };
  tubs& z        (G4D x) { half_z_    = x/2; return *this; };

  G4Tubs* solid() const;
  LOGICAL
  PLACE
private:
  G4String name;
  OPT_DOUBLE r_inner_;
  OPT_DOUBLE r_delta_;
  OPT_DOUBLE r_outer_;
  G4D   phi_start_ = 0;
  OPT_DOUBLE phi_end_;
  OPT_DOUBLE phi_delta_;
  G4D   half_z_;
  const static constexpr G4D phi_full = 360 * deg;
};

#undef LOGICAL
#undef PLACE
#undef OPT_DOUBLE
#undef G4D

}; // namespace nain4

namespace n4 { using namespace nain4; }

#endif // N4_VOLUMES_HH
