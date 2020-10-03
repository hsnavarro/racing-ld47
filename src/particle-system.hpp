#include "particle.hpp"

class ParticleSystem {
public:
  void renew();
  void update(float);
  void draw();
  void emissionFromCar(Car&);

  ParticleSystem();

  std::vector<Particle> particles;
};