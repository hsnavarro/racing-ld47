#include "particle.hpp"

#include <random>

Particle::Particle(float _lifeTime, float particleWidth, float particleHeight, ParticleType _type) : type{ _type } , lifeTime{ _lifeTime } {
  if (_type == ParticleType::SMOKE) {
    circleShape.setRadius(particleWidth);
    circleShape.setFillColor({ 0, 0, 0, 92 });
  } else {
    rectangleShape.setSize({ particleWidth, particleHeight });
    rectangleShape.setFillColor({ 0, 0, 0, 192 });
  }
}

void Particle::update(float timeElapsed) {
  lifeTime -= timeElapsed;
  if (lifeTime < 0.0f) isExpired = true;
}

sf::Shape& Particle::shape() {
  if(type == ParticleType::SMOKE) return circleShape;
  return rectangleShape;
}

void Particle::setPosition(const sf::Vector2f& position) {
  if (type == ParticleType::SMOKE) {
    const float circleRadius = circleShape.getRadius();
    circleShape.setOrigin(circleRadius * 0.5f, circleRadius * 0.5f);
    circleShape.setPosition(position);
  } else {
    const float rectangleWidth = rectangleShape.getSize().x;
    const float rectangleHeight = rectangleShape.getSize().y;
    rectangleShape.setOrigin(rectangleWidth * 0.5f, rectangleHeight * 0.5f);
    rectangleShape.setPosition(position);
  }
}
