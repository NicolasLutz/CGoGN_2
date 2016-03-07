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

#include <gtest/gtest.h>

#include <core/cmap/cmap1.h>

namespace cgogn
{

#define NB_MAX 100

/*!
 * \brief The CMap1TopoTest class implements topological tests on CMap1
 * It derives from CMap1 to allow the test of protected methods
 *
 * Note that these tests, check that the topological operators perform as wanted
 * but do neither tests the containers (refs_, used_, etc.) or the iterators.
 * These last tests are implemented in another test suite.
 */
class CMap1TopoTest : public CMap1<DefaultMapTraits>, public ::testing::Test
{

public:

	using Vertex = CMap1TopoTest::Vertex;
	using Face   = CMap1TopoTest::Face;

protected:

	/*!
	 * \brief A vector of darts on which the methods are tested.
	 */
	std::vector<Dart> darts_;

	CMap1TopoTest()
	{
		darts_.reserve(NB_MAX);
		std::srand(static_cast<unsigned int>(std::time(0)));
	}

	/*!
	 * \brief Initialize the darts in darts_ with added vertices
	 * \param n : the number of added darts or vertices
	 */
	void add_vertices(unsigned int n)
	{
		darts_.clear();
		for (unsigned int i = 0; i < n; ++i)
			darts_.push_back(add_dart());
	}

	/*!
	 * \brief Generate a random set of faces and put them in darts_
	 * \return The total number of added darts or vertices.
	 * The face size ranges from 1 to 10.
	 * A random dart of each face is put in the darts_ array.
	 */
	unsigned int add_faces(unsigned int n)
	{
		darts_.clear();
		unsigned int count = 0;
		for (unsigned int i = 0; i < n; ++i)
		{
			unsigned int n = 1 + std::rand() % 10;
			Dart d = add_face_topo(n);
			count += n;

			n = std::rand() % 10;
			while (n-- > 0)	d = phi1(d);

			darts_.push_back(d);
		}
		return count;
	}
};

/*!
 * \brief The random generated maps used in the tests are sound.
 */
TEST_F(CMap1TopoTest, random_map_generators)
{
	EXPECT_EQ(nb_darts(), 0u);

	add_vertices(NB_MAX);
	EXPECT_TRUE(check_map_integrity());

	add_faces(NB_MAX);
	EXPECT_TRUE(check_map_integrity());
}

/*!
 * \brief Sewing and unsewing darts correctly changes the topological relations.
 * The test perfoms NB_MAX sewing and unsewing on randomly chosen dart of darts_.
 * The map integrity is preserved.
 */
TEST_F(CMap1TopoTest, phi1_sew_unsew)
{
	add_vertices(NB_MAX);
	int count_faces = NB_MAX;

	for (unsigned int i = 0; i < NB_MAX; ++i)
	{
		Dart d = darts_[std::rand() % NB_MAX];
		Dart e = darts_[std::rand() % NB_MAX];
		Dart f = darts_[std::rand() % NB_MAX];
		Dart nd = phi1(d);
		Dart ne = phi1(e);
		phi1_sew(d, e);
		EXPECT_TRUE(phi1(d) == ne);
		EXPECT_TRUE(phi1(e) == nd);
		Dart nf1 = phi1(f);
		Dart nf2 = phi1(nf1);
		phi1_unsew(f);
		EXPECT_TRUE(phi1(nf1) == nf1);
		EXPECT_TRUE(phi1(f) == nf2);
	}
	EXPECT_TRUE(check_map_integrity());
}

/*!
 * \brief Adding a face of size n adds n darts, n vertices and 1 face.
 * The test adds some faces and check that the number of generated cells is correct
 * and that the map integrity is preserved.
 */
TEST_F(CMap1TopoTest, add_face_topo)
{
	add_face_topo(1);
	EXPECT_EQ(nb_darts(), 1);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), 1);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), 1);

	add_face_topo(10);
	EXPECT_EQ(nb_darts(), 11);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), 11);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), 2);

	unsigned int count_vertices = 11 + add_faces(NB_MAX);

	EXPECT_EQ(nb_darts(), count_vertices);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), count_vertices);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), NB_MAX+2);
	EXPECT_TRUE(check_map_integrity());
}

/*! \brief Removing a face removes all its vertices.
 * The test randomly removes 1/3 of the initial faces.
 * The number of cells correctly decreases and the map integrity is preserved.
 */
TEST_F(CMap1TopoTest, remove_face)
{
	unsigned int count_vertices = add_faces(NB_MAX);
	unsigned int count_faces = NB_MAX;

	for (Dart d: darts_)
	{
		if (std::rand()%3 == 1)
		{
			unsigned int k = degree(Face(d));
			remove_face(d);
			count_vertices -= k;
			--count_faces;
		}
	}
	EXPECT_EQ(nb_darts(), count_vertices);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), count_vertices);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), count_faces);
	EXPECT_TRUE(check_map_integrity());
}

/*! \brief Spliting a vertex increases the size of its face.
 * The test performs NB_MAX vertex spliting on vertices of randomly generated faces.
 * The number of generated cells is correct and the map integrity is preserved.
 */
TEST_F(CMap1TopoTest, split_vertex_topo)
{
	unsigned int count_vertices = add_faces(NB_MAX);

	for (Dart d: darts_)
	{
		unsigned int k = degree(Face(d));
		split_vertex_topo(d);
		++count_vertices;
		EXPECT_EQ(degree(Face(d)), k+1);
	}
	EXPECT_EQ(nb_darts(), count_vertices);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), count_vertices);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), NB_MAX);
	EXPECT_TRUE(check_map_integrity());
}

/*! \brief Removing a vertex decreases the size of its face and removes its if its degree is 1.
* The test performs NB_MAX vertex removing on vertices of randomly generated faces.
* The number of removed cells is correct and the map integrity is preserved.
*/
TEST_F(CMap1TopoTest, remove_vertex)
{
	unsigned int count_vertices = add_faces(NB_MAX);
	unsigned int count_faces = NB_MAX;

	for (Dart d: darts_)
	{
		unsigned int k = degree(Face(d));
		if (k > 1)
		{
			Dart e = phi1(d);
			remove_vertex(Vertex(d));
			--count_vertices;
			EXPECT_EQ(degree(Face(e)), k-1);
		}
		else
		{
			remove_vertex(Vertex(d));
			--count_faces;
			--count_vertices;
		}
	}
	EXPECT_EQ(nb_darts(), count_vertices);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), count_vertices);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), count_faces);
	EXPECT_TRUE(check_map_integrity());
}

/*! \brief Reversing a face reverses the order of its vertices.
 * The test reverses randomly generated faces.
 * The number of faces and their degrees do not change and the map integrity is preserved.
 */
TEST_F(CMap1TopoTest, reverse_face_topo)
{
	unsigned int count_vertices = add_faces(NB_MAX);

	for (Dart d: darts_)
	{
		unsigned int k = degree(Face(d));

		std::vector<Dart> face_darts;
		face_darts.reserve(k);
		foreach_dart_of_orbit(Face(d), [&] (Dart e)
		{
			face_darts.push_back(e);
		});

		reverse_face_topo(d);
		EXPECT_EQ(degree(Face(d)), k);

		d = phi1(d);
		foreach_dart_of_orbit(Face(d), [&] (Dart e)
		{
			EXPECT_TRUE(face_darts.back() == e);
			face_darts.pop_back();
		});
		EXPECT_TRUE(face_darts.empty());
	}
	EXPECT_EQ(nb_darts(), count_vertices);
	EXPECT_EQ(nb_cells<Vertex::ORBIT>(), count_vertices);
	EXPECT_EQ(nb_cells<Face::ORBIT>(), NB_MAX);
	EXPECT_TRUE(check_map_integrity());
}

/*! \brief The degree of a face is correctly computed.
 */
TEST_F(CMap1TopoTest, degree)
{
	Face f = this->add_face_topo(10);

	EXPECT_EQ(degree(f),10);
}

/*! \brief The degree of a face is correctly tested.
 */
TEST_F(CMap1TopoTest, has_degree)
{
	Face f = this->add_face_topo(10);

	EXPECT_TRUE(has_degree(f,10));
	EXPECT_FALSE(has_degree(f,0));
	EXPECT_FALSE(has_degree(f,9));
	EXPECT_FALSE(has_degree(f,11));
}

#undef NB_MAX

} // namespace cgogn
