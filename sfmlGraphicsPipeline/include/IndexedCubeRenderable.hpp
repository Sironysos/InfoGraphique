#ifndef INDEXED_CUBE_RENDERABLE_HPP
#define INDEXED_CUBE_RENDERABLE_HPP

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
        std::vector< unsigned int > m_indices;

        unsigned int m_vBuffer;
        unsigned int m_cBuffer;
        unsigned int m_iBuffer;

};

typedef std::shared_ptr<IndexedCubeRenderable> IndexedCubeRenderablePtr;

#endif
