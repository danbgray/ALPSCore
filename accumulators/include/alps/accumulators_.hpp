/*
 * Copyright (C) 1998-2014 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_WRAPED_ACCUMULATORS_HPP
#define ALPS_WRAPED_ACCUMULATORS_HPP

#include <alps/config.hpp>

#include <alps/hdf5/vector.hpp>
#include <alps/hdf5/archive.hpp>

#ifdef ALPS_HAVE_MPI
    #include <alps/utilities/boost_mpi.hpp>
#endif

#include <alps/accumulators/wrapper_set.hpp>

#include <boost/preprocessor/tuple/to_seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <string>
#include <iostream>
#include <typeinfo>
#include <stdexcept>

#ifndef ALPS_ACCUMULATOR_VALUE_TYPES_SEQ
    #define ALPS_ACCUMULATOR_VALUE_TYPES_SEQ BOOST_PP_TUPLE_TO_SEQ(ALPS_ACCUMULATOR_VALUE_TYPES_SIZE, (ALPS_ACCUMULATOR_VALUE_TYPES))
#endif

namespace alps {
    namespace accumulators {

        class ALPS_DECL accumulator_wrapper;

        namespace wrapped {

            class ALPS_DECL virtual_accumulator_wrapper {
                public:
                    // default constructor
                    virtual_accumulator_wrapper();

                    // constructor from raw accumulator
                    virtual_accumulator_wrapper(accumulator_wrapper * arg);

                    // copy constructor
                    virtual_accumulator_wrapper(virtual_accumulator_wrapper const & rhs);

                    // constructor from hdf5
                    virtual_accumulator_wrapper(hdf5::archive & ar);

                    virtual ~virtual_accumulator_wrapper();

                    // operator(), operator<<
                    #define ALPS_ACCUMULATOR_OPERATOR_CALL(r, data, T)              \
                        virtual_accumulator_wrapper & operator()(T const & value);  \
                        virtual_accumulator_wrapper & operator<<(T const & value) { \
                            (*this)(value);                                         \
                            return (*this);                                         \
                        }
                    BOOST_PP_SEQ_FOR_EACH(ALPS_ACCUMULATOR_OPERATOR_CALL, ~, ALPS_ACCUMULATOR_VALUE_TYPES_SEQ)
                    #undef ALPS_ACCUMULATOR_OPERATOR_CALL

                    /// Merge another accumulator into this one. @param rhs  accumulator to merge.
                    void merge(const virtual_accumulator_wrapper & rhs);

                    virtual_accumulator_wrapper & operator=(boost::shared_ptr<virtual_accumulator_wrapper> const & rhs);

                    // get
                    // template <typename T> base_wrapper<T> & get() {
                    //     get_visitor<T> visitor;
                    //     boost::apply_visitor(visitor, m_variant);
                    //     return *visitor.value;
                    // }

                    // extract
                    // template <typename A> A & extract() {
                    //     throw std::logic_error(std::string("unknown type : ") + typeid(A).name() + ALPS_STACKTRACE);
                    // }
                    // template <> MeanAccumulatorDouble & extract<MeanAccumulatorDouble>();

                    // count
                    boost::uint64_t count() const;

                private:

                    #define ALPS_ACCUMULATOR_MEAN_IMPL(r, data, T)  \
                        T mean_impl(T) const;
                    BOOST_PP_SEQ_FOR_EACH(ALPS_ACCUMULATOR_MEAN_IMPL, ~, ALPS_ACCUMULATOR_VALUE_TYPES_SEQ)
                    #undef ALPS_ACCUMULATOR_MEAN_IMPL

                    #define ALPS_ACCUMULATOR_ERROR_IMPL(r, data, T)  \
                        T error_impl(T) const;
                    BOOST_PP_SEQ_FOR_EACH(ALPS_ACCUMULATOR_ERROR_IMPL, ~, ALPS_ACCUMULATOR_VALUE_TYPES_SEQ)
                    #undef ALPS_ACCUMULATOR_ERROR_IMPL

                public:

                    // mean
                    template<typename T> T mean() const {
                        return mean_impl(T());
                    }

                    // error
                    template<typename T> T error() const {
                        return mean_impl(T());
                    }

                    // save
                    void save(hdf5::archive & ar) const;

                    // load
                    void load(hdf5::archive & ar);

                    // reset
                    void reset() const;

                    // result
                    // boost::shared_ptr<result_wrapper> result() const;

                    // print
                    void print(std::ostream & os) const;

#ifdef ALPS_HAVE_MPI
                    // collective_merge
                    void collective_merge(boost::mpi::communicator const & comm, int root);
                    void collective_merge(boost::mpi::communicator const & comm, int root) const;
#endif

                private:

                    std::ptrdiff_t * m_cnt;
                    accumulator_wrapper * m_ptr;
            };

            inline std::ostream & operator<<(std::ostream & os, const virtual_accumulator_wrapper & arg) {
                arg.print(os);
                return os;
            }

            // template <typename A> A & extract(virtual_accumulator_wrapper & m) {
            //     return m.extract<A>();
            // }

            inline void ALPS_DECL reset(virtual_accumulator_wrapper & arg) {
                return arg.reset();
            }

        }
    }

    // TODO: take type from variant type
    template<typename T> struct MeanAccumulator {
        public:
            MeanAccumulator(std::string const & name): m_name(name) {}
            std::string const & name() const { return m_name; }
        private:
            std::string m_name;
    };

    typedef accumulators::impl::wrapper_set<accumulators::wrapped::virtual_accumulator_wrapper> accumulator_set;
    // typedef impl::wrapper_set<result_wrapper> result_set;

    accumulator_set & operator<<(accumulator_set & set, const MeanAccumulator<double> & arg);
}

#endif