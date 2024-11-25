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
    //TODO: Implement a damped spring
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless

    //Compute displacement vector
    glm::vec3 dir = m_p2->getPosition() - m_p1->getPosition();

    //Compute displacement length
    float l=glm::length(dir);

    //Compute spring force corresponding to the displacement 
    //If the displacement is measurable by the computer (otherwise no force)
    if(l>std::numeric_limits<float>::epsilon())
    {
        return;
    }
    dir = dir/l;
    float delta_l = l - m_equilibriumLength;
    glm::vec3 force = -m_stiffness * delta_l * dir;

    //m_p1->incrForce(force);
    m_p1->incrForce(-force);
    m_p2->incrForce(force);

    glm::vec3 v = m_p2->getVelocity() - m_p1->getVelocity();
    //TODO

}

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
