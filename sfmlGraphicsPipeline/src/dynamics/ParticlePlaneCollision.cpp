#include "./../../include/dynamics/ParticlePlaneCollision.hpp"

ParticlePlaneCollision::~ParticlePlaneCollision()
{}


ParticlePlaneCollision::ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

void ParticlePlaneCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_particle->isFixed()) return;

    //TODO: Solve ParticlePlane collisions, update particle position and velocity after collision
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition(), Particle::getVelocity(), Particle::setPosition(), Particle::setVelocity()
    // Compute the distance from the particle to the plane
    float distance = glm::dot(m_particle->getPosition(), m_plane->normal()) - m_plane->distanceToOrigin();

    // Check if the particle is colliding with the plane
    if (std::abs(distance) > m_particle->getRadius()) return;

    // Project the particle onto the plane
    glm::vec3 projection = m_particle->getPosition() - distance * m_plane->normal();

    // Compute the post-collision velocity
    glm::vec3 velocity = m_particle->getVelocity();
    glm::vec3 normal = m_plane->normal();
    glm::vec3 newVelocity = velocity - (1.0f + m_restitution) * glm::dot(velocity, normal) * normal;

    // Update the particle's position and velocity
    m_particle->setPosition(projection + m_particle->getRadius() * normal);
    m_particle->setVelocity(newVelocity);
}



bool testParticlePlane(const ParticlePtr &particle, const PlanePtr &plane)
{
    /* Equation of a plane passing through A and normal to n:
   * dot( p - A, n ) = dot( p, n ) - dot( A, n ) = 0
   * dot( A, n ) is stored in the "distanceToOrigin" of the plane.
   *
   * Equation of a particle of radius r centered in c:
   * dot( p - c, p - c ) = r²
   *
   * distance( plane, particle )
   *   = min( distance( plane, c ) - r, 0 )              //definition
   *   = min( abs( dot( c - A, n ) ) - r, 0 )
   *   = min( abs( dot( c, n ) - dot( A, n ) ) - r, 0 )
   *
   * So, there is intersection if distance( plane, particle ) = 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) - r <= 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) <= r
   */

    //TODO: Test collision between particle and plane
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition()
    // Get the particle position and plane normal
    glm::vec3 particlePosition = particle->getPosition();
    glm::vec3 planeNormal = plane->normal();

    // Compute the distance from the particle to the plane
    float distance = glm::dot(particlePosition, planeNormal) - plane->distanceToOrigin();

    // Check if the distance is less than or equal to the particle radius
    return std::abs(distance) <= particle->getRadius();
}
