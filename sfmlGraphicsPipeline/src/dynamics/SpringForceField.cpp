#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2, float stiffness, float equilibriumLength, float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness),
    m_equilibriumLength(equilibriumLength),
    m_damping(damping)
{}

void SpringForceField::do_addForce()
{
    //Implement a damped spring
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless
    //Compute displacement vector
    //Compute displacement length
    //Compute spring force corresponding to the displacement 
    //If the displacement is measurable by the computer (otherwise no force)
    glm::vec3 displacement = m_p2->getPosition() - m_p1->getPosition();
    float displacementLength = glm::length(displacement);

    if (displacementLength > std::numeric_limits<float>::epsilon()) {
        glm::vec3 displacementDirection = glm::normalize(displacement);
        glm::vec3 relativeVelocity = m_p2->getVelocity() - m_p1->getVelocity();
        glm::vec3 springForce = -m_stiffness * (displacementLength - m_equilibriumLength) * displacementDirection;
        glm::vec3 dampingForce = -m_damping * relativeVelocity;

        glm::vec3 totalForce = springForce + dampingForce;

        m_p1->setForce(m_p1->getForce() + totalForce);
        m_p2->setForce(m_p2->getForce() - totalForce);
    }
}

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
