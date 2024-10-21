#ifndef CUBE_RENDERABLE_HPP
#define CUBE_RENDERABLE_HPP

#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class IndexedCubeRenderable : public Renderable
{
    public:
        ~IndexedCubeRenderable();
        IndexedCubeRenderable( ShaderProgramPtr program );

    private:
        void do_draw();

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        unsigned int m_vBuffer;
        unsigned int m_cBuffer;

};

typedef std::shared_ptr<IndexedCubeRenderable> IndexedCubeRenderablePtr;

#endif
