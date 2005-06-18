//////////////////////////////////////////////////////////////////////////////
// (c) Copyright Andreas Huber Doenni 2005
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//////////////////////////////////////////////////////////////////////////////



#include <libs/statechart/test/ThrowingBoostAssert.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/result.hpp>

#include <boost/test/test_tools.hpp>

#include <stdexcept> // std::logic_error

namespace sc = boost::statechart;



struct E : sc::event< E > {};

struct A;
struct InvalidResultCopyTest :
  sc::state_machine< InvalidResultCopyTest, A > {};

struct A : sc::simple_state< A, InvalidResultCopyTest >
{
  typedef sc::custom_reaction< E > reactions;

  sc::result react( const E & )
  {
    sc::result r( discard_event() );
    sc::result rCopy1( r );
    // Ensure the copy is consumed so that we're not accidentally tripping
    // the assert in the sc::result dtor
    sc::detail::result_utility::get_result( rCopy1 );

    // We must not make more than one copy of a result value
    sc::result rCopy2( r );
    return rCopy2;
  }
};



int test_main( int, char* [] )
{
  InvalidResultCopyTest machine;
  machine.initiate();

  #ifdef NDEBUG
    BOOST_CHECK_NO_THROW( machine.process_event( E() ) );
  #else
    BOOST_CHECK_THROW( machine.process_event( E() ), std::logic_error );
  #endif

  return 0;
}