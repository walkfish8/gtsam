/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file    NonlinearFactorGraph.h
 * @brief   Factor Graph Constsiting of non-linear factors
 * @author  Frank Dellaert
 * @author  Carlos Nieto
 * @author  Christian Potthast
 */

// \callgraph

#pragma once

#include <gtsam/inference/SymbolicFactorGraph.h>
#include <gtsam/linear/GaussianFactorGraph.h>
#include <gtsam/nonlinear/NonlinearFactor.h>

namespace gtsam {

	/**
	 * A non-linear factor graph is a graph of non-Gaussian, i.e. non-linear factors,
	 * which derive from NonlinearFactor. The values structures are typically (in SAM) more general
	 * than just vectors, e.g., Rot3 or Pose3, which are objects in non-linear manifolds.
	 * Linearizing the non-linear factor graph creates a linear factor graph on the 
	 * tangent vector space at the linearization point. Because the tangent space is a true
	 * vector space, the config type will be an VectorValues in that linearized factor graph.
	 */
	class NonlinearFactorGraph: public FactorGraph<NonlinearFactor> {

	public:

	  typedef FactorGraph<NonlinearFactor> Base;
	  typedef boost::shared_ptr<NonlinearFactorGraph> shared_ptr;
		typedef boost::shared_ptr<NonlinearFactor> sharedFactor;

    /** print just calls base class */
    void print(const std::string& str = "NonlinearFactorGraph: ") const;

    /** return keys in some random order */
    std::set<Symbol> keys() const;

		/** unnormalized error */
		double error(const DynamicValues& c) const;

		/** Unnormalized probability. O(n) */
		double probPrime(const DynamicValues& c) const;

		template<class F>
		void add(const F& factor) {
			this->push_back(boost::shared_ptr<F>(new F(factor)));
		}

		/**
		 * Create a symbolic factor graph using an existing ordering
		 */
		SymbolicFactorGraph::shared_ptr symbolic(const Ordering& ordering) const;

		/**
		 * Create a symbolic factor graph and initial variable ordering that can
		 * be used for graph operations like determining a fill-reducing ordering.
		 * The graph and ordering should be permuted after such a fill-reducing
		 * ordering is found.
		 */
		std::pair<SymbolicFactorGraph::shared_ptr, Ordering::shared_ptr>
		symbolic(const DynamicValues& config) const;

    /**
     * Compute a fill-reducing ordering using COLAMD.  This returns the
     * ordering and a VariableIndex, which can later be re-used to save
     * computation.
     */
		Ordering::shared_ptr orderingCOLAMD(const DynamicValues& config) const;

		/**
		 * linearize a nonlinear factor graph
		 */
		boost::shared_ptr<GaussianFactorGraph >
				linearize(const DynamicValues& config, const Ordering& ordering) const;

	private:

		/** Serialization function */
		friend class boost::serialization::access;
		template<class ARCHIVE>
		void serialize(ARCHIVE & ar, const unsigned int version) {
			ar & boost::serialization::make_nvp("NonlinearFactorGraph",
								boost::serialization::base_object<Base>(*this));
		}
	};

} // namespace

