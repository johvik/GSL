///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include <UnitTest++/UnitTest++.h>

#include <cstdlib>
#include <gsl/string_span>
#include <boost/assign.hpp>
#include <vector>
#include <map>

using namespace std;
using namespace gsl;


SUITE(string_span_tests)
{

    TEST(TestLiteralConstruction)
    {
        cwstring_span<>::type v = ensure_z(L"Hello");
        CHECK(5 == v.length());

#ifdef CONFIRM_COMPILATION_ERRORS
        wstring_span<>::type v2 = ensure0(L"Hello");
#endif
    }

    TEST(TestConstructFromStdString)
    {
        std::string s = "Hello there world";
        cstring_span<>::type v = s;
        CHECK(v.length() == static_cast<cstring_span<>::type::index_type>(s.length()));
    }

    TEST(TestConstructFromStdVector)
    {
        std::vector<char> vec(5, 'h');
        string_span<>::type v (vec);
        CHECK(v.length() == static_cast<string_span<>::type::index_type>(vec.size()));
    }

    TEST(TestStackArrayConstruction)
    {
        wchar_t stack_string[] = L"Hello";

        {
            cwstring_span<>::type v = ensure_z(stack_string);
            CHECK(v.length() == 5);
        }
        {
            cwstring_span<>::type v = stack_string;
            CHECK(v.length() == 5);
        }

        {
            wstring_span<>::type v = ensure_z(stack_string);
            CHECK(v.length() == 5);
        }

        {
            wstring_span<>::type v = stack_string;
            CHECK(v.length() == 5);
        }
    }

    TEST(TestConstructFromConstCharPointer)
    {
        const char* s = "Hello";
        cstring_span<>::type v = ensure_z(s);
        CHECK(v.length() == 5);
    }

    TEST(TestConversionToConst)
    {
        char stack_string[] = "Hello";
        string_span<>::type v = ensure_z(stack_string);
        cstring_span<>::type v2 = v;
        CHECK(v.length() == v2.length());
    }

    TEST(TestConversionFromConst)
    {
        char stack_string[] = "Hello";
        cstring_span<>::type v = ensure_z(stack_string);
        (void)v;
#ifdef CONFIRM_COMPILATION_ERRORS
        string_span<>::type v2 = v;
        string_span<>::type v3 = "Hello";
#endif
    }

    TEST(TestToString)
    {
        std::string s = gsl::to_string(cstring_span<>::type());
        CHECK(s.length() == 0);

        char stack_string[] = "Hello";
        cstring_span<>::type v = ensure_z(stack_string);
        std::string s2 = gsl::to_string(v);
        CHECK(static_cast<cstring_span<>::type::index_type>(s2.length()) == v.length());
        CHECK(s2.length() == 5);
    }

    TEST(EqualityAndImplicitConstructors)
    {
        {
            cstring_span<>::type span = "Hello";
            cstring_span<>::type span1;

            // comparison to empty span
            CHECK(span1 != span);
            CHECK(span != span1);
        }

        {
            cstring_span<>::type span = "Hello";
            cstring_span<>::type span1 = "Hello1";

            // comparison to different span
            CHECK(span1 != span);
            CHECK(span != span1);
        }

        {
            cstring_span<>::type span = "Hello";

            const char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            const char ar1[] = "Hello";
            const char ar2[10] = "Hello";
            const char* ptr = "Hello";
            const std::string str = "Hello";
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            gsl::span<const char> sp = ensure_z("Hello");

            // comparison to  literal
            CHECK(span == cstring_span<>::type("Hello"));

            // comparison to static array with no null termination
            CHECK(span == cstring_span<>::type(ar));

            // comparison to static array with null at the end
            CHECK(span == cstring_span<>::type(ar1));

            // comparison to static array with null in the middle
            CHECK(span == cstring_span<>::type(ar2));

            // comparison to null-terminated c string
            CHECK(span == cstring_span<>::type(ptr, 5));

            // comparison to string
            CHECK(span == cstring_span<>::type(str));

            // comparison to vector of charaters with no null termination
            CHECK(span == cstring_span<>::type(vec));

            // comparison to span
            CHECK(span == cstring_span<>::type(sp));

            // comparison to string_span
            CHECK(span == span);
        }

        {
            char ar[] = { 'H', 'e', 'l', 'l', 'o' };

            string_span<>::type span = ar;

            char ar1[] = "Hello";
            char ar2[10] = "Hello";
            char* ptr = ar;
            std::string str = "Hello";
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            gsl::span<char> sp = ensure_z(ar1);

            // comparison to static array with no null termination
            CHECK(span == string_span<>::type(ar));

            // comparison to static array with null at the end
            CHECK(span == string_span<>::type(ar1));

            // comparison to static array with null in the middle
            CHECK(span == string_span<>::type(ar2));

            // comparison to null-terminated c string
            CHECK(span == string_span<>::type(ptr, 5));

            // comparison to string
            CHECK(span == string_span<>::type(str));

            // comparison to vector of charaters with no null termination
            CHECK(span == string_span<>::type(vec));

            // comparison to span
            CHECK(span == string_span<>::type(sp));

            // comparison to string_span
            CHECK(span == span);
        }


        {
            const char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            const char ar1[] = "Hello";
            const char ar2[10] = "Hello";
            const std::string str = "Hello";
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            gsl::span<const char> sp = ensure_z("Hello");

            cstring_span<>::type span = "Hello";

            // const span, const other type

            CHECK(span == "Hello");
            CHECK(span == ar);
            CHECK(span == ar1);
            CHECK(span == ar2);
#ifdef CONFIRM_COMPILATION_ERRORS
            const char* ptr = "Hello";
            CHECK(span == ptr);
#endif
            CHECK(span == str);
            CHECK(span == vec);
            CHECK(span == sp);

            CHECK("Hello" == span);
            CHECK(ar == span);
            CHECK(ar1 == span);
            CHECK(ar2 == span);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(ptr == span);
#endif
            CHECK(str == span);
            CHECK(vec == span);
            CHECK(sp == span);

            // const span, non-const other type

            char _ar[] = { 'H', 'e', 'l', 'l', 'o' };
            char _ar1[] = "Hello";
            char _ar2[10] = "Hello";
            char* _ptr = _ar;
            std::string _str = "Hello";
            std::vector<char> _vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            gsl::span<char> _sp( _ar, 5 );

            CHECK(span == _ar);
            CHECK(span == _ar1);
            CHECK(span == _ar2);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(span == _ptr);
#endif
            CHECK(span == _str);
            CHECK(span == _vec);
            CHECK(span == _sp);

            CHECK(_ar == span);
            CHECK(_ar1 == span);
            CHECK(_ar2 == span);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(_ptr == span);
#endif
            CHECK(_str == span);
            CHECK(_vec == span);
            CHECK(_sp == span);

            string_span<>::type _span( _ptr, 5 );

            // non-const span, non-const other type

            CHECK(_span == _ar);
            CHECK(_span == _ar1);
            CHECK(_span == _ar2);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(_span == _ptr);
#endif
            CHECK(_span == _str);
            CHECK(_span == _vec);
            CHECK(_span == _sp);

            CHECK(_ar == _span);
            CHECK(_ar1 == _span);
            CHECK(_ar2 == _span);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(_ptr == _span);
#endif
            CHECK(_str == _span);
            CHECK(_vec == _span);
            CHECK(_sp == _span);

            // non-const span, const other type

            CHECK(_span == "Hello");
            CHECK(_span == ar);
            CHECK(_span == ar1);
            CHECK(_span == ar2);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(_span == ptr);
#endif
            CHECK(_span == str);
            CHECK(_span == vec);
            CHECK(_span == sp);

            CHECK("Hello" == _span);
            CHECK(ar == _span);
            CHECK(ar1 == _span);
            CHECK(ar2 == _span);
#ifdef CONFIRM_COMPILATION_ERRORS
            CHECK(ptr == _span);
#endif
            CHECK(str == _span);
            CHECK(vec == _span);
            CHECK(sp == _span);

            // two spans

            CHECK(_span == span);
            CHECK(span == _span);
        }

        {
            std::vector<char> str1 = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            cstring_span<>::type span1 = str1;
            std::vector<char> str2 = std::move(str1);
            cstring_span<>::type span2 = str2;

            // comparison of spans from the same vector before and after move (ok)
            CHECK(span1 == span2);
        }
    }

    TEST(ComparisonAndImplicitConstructors)
    {
        {
            cstring_span<>::type span = "Hello";

            const char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            const char ar1[] = "Hello";
            const char ar2[10] = "Hello";
            const char* ptr = "Hello";
            const std::string str = "Hello";
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );

            // comparison to  literal
            CHECK(span < cstring_span<>::type("Helloo"));
            CHECK(span > cstring_span<>::type("Hell"));

            // comparison to static array with no null termination
            CHECK(span >= cstring_span<>::type(ar));

            // comparison to static array with null at the end
            CHECK(span <= cstring_span<>::type(ar1));

            // comparison to static array with null in the middle
            CHECK(span >= cstring_span<>::type(ar2));

            // comparison to null-terminated c string
            CHECK(span <= cstring_span<>::type(ptr, 5));

            // comparison to string
            CHECK(span >= cstring_span<>::type(str));

            // comparison to vector of charaters with no null termination
            CHECK(span <= cstring_span<>::type(vec));
        }

        {
            char ar[] = { 'H', 'e', 'l', 'l', 'o' };

            string_span<>::type span = ar;

            char larr[] = "Hell";
            char rarr[] = "Helloo";

            char ar1[] = "Hello";
            char ar2[10] = "Hello";
            char* ptr = ar;
            std::string str = "Hello";
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );


            // comparison to static array with no null termination
            CHECK(span <= string_span<>::type(ar));
            CHECK(span < string_span<>::type(rarr));
            CHECK(span > string_span<>::type(larr));

            // comparison to static array with null at the end
            CHECK(span >= string_span<>::type(ar1));

            // comparison to static array with null in the middle
            CHECK(span <= string_span<>::type(ar2));

            // comparison to null-terminated c string
            CHECK(span >= string_span<>::type(ptr, 5));

            // comparison to string
            CHECK(span <= string_span<>::type(str));

            // comparison to vector of charaters with no null termination
            CHECK(span >= string_span<>::type(vec));
        }
    }
    TEST(ConstrutorsEnsureZ)
    {
        // remove z from literals
        {
            cstring_span<>::type sp = "hello";
            CHECK((sp.length() == 5));
        }

        // take the string as is
        {
            std::string str = std::string("hello");
            cstring_span<>::type sp = str;
            CHECK((sp.length() == 5));
        }

        // ensure z on c strings
        {
            char* ptr = new char[3];

            ptr[0] = 'a';
            ptr[1] = 'b';
            ptr[2] = '\0';

            string_span<>::type span = ensure_z(ptr);
            CHECK(span.length() == 2);

            delete[] ptr;
        }
    }

    TEST(Constructors)
    {
        // creating cstring_span

        // from span of a final extent
        {
            span<const char, 6> sp = "Hello";
            cstring_span<>::type span = sp;
            CHECK(span.length() == 6);
        }

        // from const span of a final extent to non-const string_span
#ifdef CONFIRM_COMPILATION_ERRORS
        {
            span<const char, 6> sp = "Hello";
            string_span<>::type span = sp;
            CHECK(span.length() == 6);
        }
#endif

        // from string temporary
#ifdef CONFIRM_COMPILATION_ERRORS
        {
            cstring_span<>::type span = std::string("Hello");
        }
#endif

        // default
        {
            cstring_span<>::type span;
            CHECK(span.length() == 0);
        }

        // from NULLPTR
        {
            cstring_span<>::type span(NULLPTR);
            CHECK(span.length() == 0);
        }

        // from string literal
        {
            cstring_span<>::type span = "Hello";
            CHECK(span.length() == 5);
        }

        // from const static array
        {
            const char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            cstring_span<>::type span = ar;
            CHECK(span.length() == 5);
        }

        // from non-const static array
        {
            char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            cstring_span<>::type span = ar;
            CHECK(span.length() == 5);
        }

        // from const ptr and length
        {
            const char* ptr = "Hello";
            cstring_span<>::type span( ptr, 5 );
            CHECK(span.length() == 5);
        }

        // from const ptr and length, include 0
        {
            const char* ptr = "Hello";
            cstring_span<>::type span( ptr, 6 );
            CHECK(span.length() == 6);
        }

        // from const ptr and length, 0 inside
        {
            const char* ptr = "He\0lo";
            cstring_span<>::type span( ptr, 5 );
            CHECK(span.length() == 5);
        }

        // from non-const ptr and length
        {
            char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            char* ptr = ar;
            cstring_span<>::type span( ptr, 5 );
            CHECK(span.length() == 5);
        }

        // from non-const ptr and length, 0 inside
        {
            char ar[] = { 'H', 'e', '\0', 'l', 'o' };
            char* ptr = ar;
            cstring_span<>::type span( ptr, 5 );
            CHECK(span.length() == 5);
        }

        // from const string
        {
            const std::string str = "Hello";
            cstring_span<>::type span = str;
            CHECK(span.length() == 5);
        }

        // from non-const string
        {
            std::string str = "Hello";
            cstring_span<>::type span = str;
            CHECK(span.length() == 5);
        }

        // from const vector
        {
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            cstring_span<>::type span = vec;
            CHECK(span.length() == 5);
        }

        // from non-const vector
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            cstring_span<>::type span = vec;
            CHECK(span.length() == 5);
        }

        // from const span
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            const span<const char> inner = vec;
            cstring_span<>::type span = inner;
            CHECK(span.length() == 5);
        }

        // from non-const span
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            span<char> inner = vec;
            cstring_span<>::type span = inner;
            CHECK(span.length() == 5);
        }

        // from const string_span
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            cstring_span<>::type tmp = vec;
            cstring_span<>::type span = tmp;
            CHECK(span.length() == 5);
        }

        // from non-const string_span
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type tmp = vec;
            cstring_span<>::type span = tmp;
            CHECK(span.length() == 5);
        }

        // creating string_span

        // from string literal
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            string_span<>::type span = "Hello";
#endif
        }

        // from const static array
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            const char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            string_span<>::type span = ar;
            CHECK(span.length() == 5);
#endif
        }

        // from non-const static array
        {
            char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            string_span<>::type span = ar;
            CHECK(span.length() == 5);
        }

        // from const ptr and length
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            const char* ptr = "Hello";
            string_span<>::type span( ptr, 5 );
            CHECK(span.length() == 5);
#endif
        }

        // from non-const ptr and length
        {
            char ar[] = { 'H', 'e', 'l', 'l', 'o' };
            char* ptr = ar;
            string_span<>::type span( ptr, 5 );
            CHECK(span.length() == 5);
        }

        // from const string
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            const std::string str = "Hello";
            string_span<>::type span = str;
            CHECK(span.length() == 5);
#endif
        }

        // from non-const string
        {
            std::string str = "Hello";
            string_span<>::type span = str;
            CHECK(span.length() == 5);
        }

        // from const vector
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type span = vec;
            CHECK(span.length() == 5);
#endif
        }

        // from non-const vector
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type span = vec;
            CHECK(span.length() == 5);
        }

        // from const span
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            const span<const char> inner = vec;
            string_span<>::type span = inner;
            CHECK(span.length() == 5);
#endif
        }

        // from non-const span
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            span<char> inner = vec;
            string_span<>::type span = inner;
            CHECK(span.length() == 5);
        }

        // from non-const span of non-const data from const vector
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            const span<char> inner = vec;
            string_span<>::type span = inner;
            CHECK(span.length() == 5);
#endif
        }

        // from const string_span
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            cstring_span<>::type tmp = vec;
            string_span<>::type span = tmp;
            CHECK(span.length() == 5);
#endif
        }

        // from non-const string_span
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type tmp = vec;
            string_span<>::type span = tmp;
            CHECK(span.length() == 5);
        }

        // from non-const string_span from const vector
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            const std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type tmp = vec;
            string_span<>::type span = tmp;
            CHECK(span.length() == 5);
#endif
        }

        // from const string_span of non-const data
        {
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            const string_span<>::type tmp = vec;
            string_span<>::type span = tmp;
            CHECK(span.length() == 5);
        }
    }

    template<typename T>
    T move_wrapper(BOOST_FWD_REF(T) t)
    {
        return boost::move(t);
    }

    template <class T>
    T create() { return T(); }

    template <class T>
    void use(basic_string_span<T, gsl::dynamic_extent> s) {}

    TEST(MoveConstructors)
    {
        // move string_span
        {
            cstring_span<>::type span = "Hello";
            cstring_span<>::type span1 = std::move(span);
            CHECK(span1.length() == 5);
        }
        {
            cstring_span<>::type span = "Hello";
            cstring_span<>::type span1 = move_wrapper(std::move(span));
            CHECK(span1.length() == 5);
        }
        {
            cstring_span<>::type span = "Hello";
            cstring_span<>::type span1 = move_wrapper(std::move(span));
            CHECK(span1.length() == 5);
        }

        // move span
        {
            span<const char> span = ensure_z("Hello");
            cstring_span<>::type span1 = std::move(span);
            CHECK(span1.length() == 5);
        }
        {
            span<const char> span = ensure_z("Hello");
            cstring_span<>::type span2 = move_wrapper(std::move(span));
            CHECK(span2.length() == 5);
        }

        // move string
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            std::string str = "Hello";
            string_span<>::type span = std::move(str);
            CHECK(span.length() == 5);
#endif
        }
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            std::string str = "Hello";
            string_span<>::type span = move_wrapper<std::string>(std::move(str));
            CHECK(span.length() == 5);
#endif
        }
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            use<char>(create<string>());
#endif
        }

        // move container
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type span = std::move(vec);
            CHECK(span.length() == 5);
#endif
        }
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            std::vector<char> vec = boost::assign::list_of( 'H')('e')('l')('l')('o' );
            string_span<>::type span = move_wrapper<std::vector<char>>(std::move(vec));
            CHECK(span.length() == 5);
#endif
        }
        {
#ifdef CONFIRM_COMPILATION_ERRORS
            use<char>(create<std::vector<char>>());
#endif
        }
    }

    TEST(Conversion)
    {
#ifdef CONFIRM_COMPILATION_ERRORS
        cstring_span<>::type span = "Hello";
        cwstring_span<>::type wspan( span );
        CHECK(wspan.length() == 5);
#endif
    }

#if __cplusplus >= 201102L
    czstring_span<>::type CreateTempName(string_span<>::type span)
    {
        Expects(span.size() > 1);

        int last = 0;
        if (span.size() > 4)
        {
            span[0] = 't';
            span[1] = 'm';
            span[2] = 'p';
            last = 3;
        }
        span[last] = '\0';

        zstring_span<>::type ret = span.subspan(0, 4);
        return{ ret };
    }
#endif
    TEST(zstring)
    {

        // create zspan from zero terminated string
#if __cplusplus >= 201102L
        {
            char buf[1];
            buf[0] = '\0';

            zstring_span<>::type zspan( buf, 1 );

            CHECK(strlen(zspan.assume_z()) == 0);
            CHECK(zspan.as_string_span().size() == 0);
            CHECK(zspan.ensure_z().size() == 0);
        }
#endif

        // create zspan from non-zero terminated string
#if __cplusplus >= 201102L
        {
            char buf[1];
            buf[0] = 'a';

            auto workaround_macro = [&]() { zstring_span<>::type zspan({ buf, 1 }); };
            CHECK_THROW(workaround_macro(), fail_fast);
        }
#endif

        // usage scenario: create zero-terminated temp file name and pass to a legacy API
#if __cplusplus >= 201102L
        {
            char buf[10];

            auto name = CreateTempName({ buf, 10 });
            if (!name.empty())
            {
                czstring<> str = name.assume_z();
                CHECK(strlen(str) == 3);
                CHECK(*(str+3) == '\0');
            }
        }
#endif

    }

#if __cplusplus >= 201102L
    cwzstring_span<>::type CreateTempNameW(wstring_span<>::type span)
    {
        Expects(span.size() > 1);

        int last = 0;
        if (span.size() > 4)
        {
            span[0] = L't';
            span[1] = L'm';
            span[2] = L'p';
            last = 3;
        }
        span[last] = L'\0';

        wzstring_span<>::type ret = span.subspan(0, 4);
        return{ ret };
    }
#endif
    TEST(wzstring)
    {

        // create zspan from zero terminated string
#if __cplusplus >= 201102L
        {
            wchar_t buf[1];
            buf[0] = L'\0';

            wzstring_span<>::type zspan( buf, 1 );

            CHECK(wcsnlen(zspan.assume_z(), 1) == 0);
            CHECK(zspan.as_string_span().size() == 0);
            CHECK(zspan.ensure_z().size() == 0);
        }

        // create zspan from non-zero terminated string
        {
            wchar_t buf[1];
            buf[0] = L'a';

            auto workaround_macro = [&]() { wzstring_span<>::type zspan({ buf, 1 }); };
            CHECK_THROW(workaround_macro(), fail_fast);
        }
#endif
        // usage scenario: create zero-terminated temp file name and pass to a legacy API
#if __cplusplus >= 201102L
        {
            wchar_t buf[10];

            auto name = CreateTempNameW({ buf, 10 });
            if (!name.empty())
            {
                cwzstring<> str = name.assume_z();
                CHECK(wcsnlen(str, 10) == 3);
                CHECK(*(str + 3) == L'\0');
            }
        }
#endif
    }

#if __cplusplus >= 201102L
    TEST(Issue305)
    {
        std::map<gsl::cstring_span<>::type, int> foo = { { "foo", 0 },{ "bar", 1 } };
        CHECK(foo["foo"] == 0);
        CHECK(foo["bar"] == 1);
    }
#endif
}

int main(int, const char *[])
{
    return UnitTest::RunAllTests();
}
