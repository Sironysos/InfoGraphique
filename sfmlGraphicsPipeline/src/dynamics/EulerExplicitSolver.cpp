#include "./../../include/dynamics/EulerExplicitSolver.hpp"

EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<ParticlePtr>& particles)
{
    for(ParticlePtr p : particles)
    {
        if(!p->isFixed())
        {
            //Implement explicit euler solver
            p->setVelocity(p->getVelocity() + p->getForce() / p->getMass() * dt);
            p->setPosition(p->getPosition() + p->getVelocity() * dt);
        }
    }
}
