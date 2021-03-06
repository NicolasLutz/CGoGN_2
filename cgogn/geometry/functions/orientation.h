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

#ifndef CGOGN_GEOMETRY_ORIENTATION_H_
#define CGOGN_GEOMETRY_ORIENTATION_H_

#include <cgogn/geometry/types/plane_3d.h>

namespace cgogn
{

namespace geometry
{

/**
 * return the orientation of point P w.r.t. the plane defined by 3 points
 * @param P the point
 * @param A plane point 1
 * @param B plane point 2
 * @param C plane point 3
 * @return the orientation
 */
template <typename VEC3>
Orientation3D test_orientation_3D(const VEC3& P, const VEC3& A, const VEC3& B, const VEC3& C)
{
	static_assert(vector_traits<VEC3>::SIZE == 3ul, "The size of the vector must be equal to 3.");
	return Plane3D<VEC3>(A, B, C).orient(P);
}

/**
 * return the orientation of point P w.r.t. the plane defined by its normal and 1 point
 * @param P the point
 * @param N plane normal
 * @param PP plane point
 * @return the orientation
 */
template <typename VEC3>
Orientation3D test_orientation_3D(const VEC3& P, const VEC3& N, const VEC3& PP)
{
	static_assert(vector_traits<VEC3>::SIZE == 3ul, "The size of the vector must be equal to 3.");
	return Plane3D<VEC3>(N, PP).orient(P);
}

} // namespace geometry

} // namespace cgogn

#endif // CGOGN_GEOMETRY_ORIENTATION_H_
