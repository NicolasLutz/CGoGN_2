/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* Copyright (C) 2015, IGG Group, ICube, University of Strasbourg, France       *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: http://cgogn.unistra.fr/                                           *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#ifndef RENDERING_DRAWER_H_
#define RENDERING_DRAWER_H_

#include <rendering/shaders/shader_color_per_vertex.h>
#include <rendering/shaders/shader_flat.h>
#include <rendering/shaders/vbo.h>
#include <rendering/dll.h>

//#include <geometry/types/vec.h>
namespace cgogn
{

namespace rendering
{


class CGOGN_RENDERING_API Drawer
{
	struct PrimParam
	{
		unsigned int begin;
		GLenum mode;
		float width;
		unsigned int nb;
		PrimParam(unsigned int b, GLenum m, float w): begin(b),mode(m),width(w),nb(0) {}
	};

	using Vec3f = std::array<float,3>;

protected:
	VBO* vbo_pos_;
	VBO* vbo_col_;
	unsigned int vao_;

	std::vector<Vec3f> data_pos_;
	std::vector<Vec3f> data_col_;
	std::vector<PrimParam> begins_;
	std::vector<PrimParam> begins_point_;
	std::vector<PrimParam> begins_line_;
	std::vector<PrimParam> begins_face_;

	float current_size_;
	static ShaderColorPerVertex* shader_cpv_;

public:

	/**
	 * constructor, init all buffers (data and OpenGL) and shader
	 * @param lineMode 0:simple thin Line / 1:line with possible width /2:3D Lines
	 * @Warning need OpenGL context
	 */
	Drawer();

	/**
	 * release buffers and shader
	 */
	~Drawer();


	/**
	 * init the data structure
	 */
	void new_list();

	/**
	 * as glBegin, but need a newList call before
	 * @param mode: GL_POINTS, GL_LINES, GL_LINE_LOOP, GL_TRIANGLES,
	 */
	void begin(GLenum mode);

	/**
	 * as glEnd
	 */
	void end();

	/**
	 * finalize the data initialization
	 * drawn is done if newList called with GL_COMPILE_AND_EXECUTE
	 */
	void end_list();

	/**
	 * use as glVertex3f
	 */
	void vertex3f(float x, float y, float z);

	/**
	 * use as glColor3f
	 */
	void color3f(float r, float g, float b);

	template <typename SCAL>
	inline void vertex3fv(SCAL* xyz)
	{
		vertex3f(float(xyz[0]),float(xyz[1]),float(xyz[2]));
	}

	template <typename SCAL>
	inline void color3fv(SCAL* rgb)
	{
		color3f(float(rgb[0]),float(rgb[1]),float(rgb[2]));
	}

	/**
	 * use as a glCallList
	 * @param projection projection matrix
	 * @param modelview modelview matrix
	 */
	void callList(const QMatrix4x4& projection, const QMatrix4x4& modelview);

	/**
	 * usr as glPointSize
	 */
	inline void pointSize(float ps)
	{
		current_size_ = ps;
	}

};



} // namespace rendering

} // namespace cgogn

#endif // RENDERING_DRAWER_H_
