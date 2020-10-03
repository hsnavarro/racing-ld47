#include "particle.hpp"

class ParticleSystem {
public:
  ParticleSystem();

  void renew();
  void update(float);
  void draw();
  void emissionFromCar(Car&);

  std::vector<Particle> particles;
};
