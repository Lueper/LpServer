/* Policies for result and outcome
(C) 2018-2024 Niall Douglas <http://www.nedproductions.biz/> (4 commits)
File Created: Sep 2018


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_OUTCOME_POLICY_FAIL_TO_COMPILE_OBSERVERS_HPP
#define BOOST_OUTCOME_POLICY_FAIL_TO_COMPILE_OBSERVERS_HPP

#include "base.hpp"

BOOST_OUTCOME_V2_NAMESPACE_EXPORT_BEGIN

#define BOOST_OUTCOME_FAIL_TO_COMPILE_OBSERVERS_MESSAGE                                                                                                                                                                                                                                                                              \
  "Attempt to wide observe value, error or "                                                                                                                                                                                                                                                                                   \
  "exception for a basic_result/basic_outcome given an EC or EP type which is not void, and for whom "                                                                                                                                                                                                                         \
  "trait::is_error_code_available<EC>, trait::is_exception_ptr_available<EC>, and trait::is_exception_ptr_available<EP> "                                                                                                                                                                                                      \
  "are all false. Please specify a NoValuePolicy to tell basic_result/basic_outcome what to do, or else use "                                                                                                                                                                                                                  \
  "a more specific convenience type alias such as unchecked<T, E> to indicate you want the wide "                                                                                                                                                                                                                              \
  "observers to be narrow, or checked<T, E> to indicate you always want an exception throw etc."

namespace policy
{
  struct fail_to_compile_observers : base
  {
    template <class Impl> static constexpr void wide_value_check(Impl && /* unused */) { static_assert(!std::is_same<Impl, Impl>::value, BOOST_OUTCOME_FAIL_TO_COMPILE_OBSERVERS_MESSAGE); }
    template <class Impl> static constexpr void wide_error_check(Impl && /* unused */) { static_assert(!std::is_same<Impl, Impl>::value, BOOST_OUTCOME_FAIL_TO_COMPILE_OBSERVERS_MESSAGE); }
    template <class Impl> static constexpr void wide_exception_check(Impl && /* unused */) { static_assert(!std::is_same<Impl, Impl>::value, BOOST_OUTCOME_FAIL_TO_COMPILE_OBSERVERS_MESSAGE); }
  };
}  // namespace policy

#undef BOOST_OUTCOME_FAIL_TO_COMPILE_OBSERVERS_MESSAGE

BOOST_OUTCOME_V2_NAMESPACE_END

#endif
