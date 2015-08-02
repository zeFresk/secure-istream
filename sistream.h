// Secure input header.
// It contains the definition of sostream, which is a secured version of istream.
// This class is a high level class so it will not provide much functions.
// I know this could have been better but if you want to code a better version feel free to do so.

// This code is under MIT license.
// by zeFresk, 2015

#ifndef SIOSTREAMMM
#define SIOSTREAMMM

#include <iostream>
#include <string>
#include <stdexcept>
#include <cassert>
#include <limits>
#include <functional>
#include <array>
#include <type_traits>

/**
 * @brief output_object
 * @typedef type : give basic_string associated type. (std::string or std::wstring)
 * @var object : give the output console object associated. (std::cout or std::wcout)
 *
 * Gives the type of basic_string associated (string/wstring) with template parameter and console output object (such as cout/wcout).
 *
 */
template <typename T>
struct output_object;
template <>
struct output_object<char>
{
    typedef typename std::string type;
    std::ostream& object = std::cout;
};
template <>
struct output_object<wchar_t>
{
    typedef typename std::wstring type;
    std::wostream& object = std::wcout;
};

/**
 * @brief error_message
 *
 * Return the default error message.
 *
 */
template <typename CharT>
typename output_object<CharT>::type error_message() {}
template <>
typename output_object<char>::type error_message<char>() {return "Please enter a correct value.";}
template <>
typename output_object<wchar_t>::type error_message<wchar_t>() {return L"Please enter a correct value.";}

/**
 * @brief default_failure
 *
 * Default on_failure function.
 *
 */
template <typename CharT>
struct default_failure
{
    void operator()(typename output_object<CharT>::type const&) const {}
};

/**
 * @brief make_array
 * @param[in] ...values : set of values to put in a std::array
 *
 * @pre all values as to be convertible to an unique type.
 *
 * @return an std::array filled with values.
 */
template <typename... Args>
constexpr auto make_array(Args&&... values) -> std::array<typename std::common_type<Args...>::type, sizeof...(values)> //tricky way to std::array
{
    return {std::forward<Args>(values)...}; //double "{" to ensure initalizer-list.
}

template <typename Stream, typename Input_Type>
bool nothrowoperator(Stream& is, Input_Type& user_input) noexcept
{
    try
    {
        is >> user_input;
        return true;
    }
    catch (std::exception&)
    {
        return false;
    }
}



/**
 * @class sistream
 * @brief secure istream class.
 *
 * High level version of istream which provide strong garantee and force user to enter a correct input.
 * It was made to be as easy to use as possible.
 */
template <class CharT, class Func, class Traits = std::char_traits<CharT>>
class basic_sistream
{
public:
    //type alias
    typedef std::basic_istream<CharT, Traits> istream_type;
    typedef typename output_object<CharT>::type string_type;
    typedef std::basic_ostream<CharT, Traits> ostream_type;

    /**
     * @brief constructor
     * @param[in] is : input stream.
     * @param[in] error_message : text which is going to be outputted if the user's input isn't valid. (std::cout)
     * @param[in] on_failure : Func to call on user's bad input. Can take ONLY ONE parameter which has to be a std::string which whill contains user input. Return of on_failure will be ignored. Func can be any callable thing.
     *
     * @pre "is" has to be a console stream such as cin, else it leads to an undefined behaviour. "error_message" can't be empty.
     *
     */
    basic_sistream(istream_type& is, string_type const& error_message = error_message<CharT>(), Func && on_failure = default_failure<CharT>{}) : m_is(is), m_on_failure_msg(error_message), m_on_failure(on_failure)
    {
        assert(!error_message.empty() && "Error in sistream ctor : \"error_message\" is empty");
    }

    /**
     * @brief raw_istream
     * @return encapsuled std::istream
     */
    istream_type& raw_istream() const {return m_is;}

    /**
     * @brief operator >>
     * @param[out] user_var : variable which is going to contains user's good input.
     *
     * @pre user_var has to implement an operator>>(istream&, var_type).
     *
     * @return sis
     *
     * Same behaviour as operator>>(istream&, ...) but guarantee than at end of func, user_var will be in a right state.
     *
     */
    template <typename Input_Type>
    basic_sistream& operator>>(Input_Type& user_var)
    {

        while (!nothrowoperator(m_is, user_var))
        {
            if (m_is.eof()) //End-Of-File
            {
                throw std::ios_base::failure("Error in operator>>(sistream&...) : EOF.");
            }
            else if (m_is.bad()) //Bad bit
            {
                throw std::ios_base::failure("Error in operatro>>(sistream&...) : Bad bit.");
            }
            else //bad user's input
            {
                //backup the bad user's input
                string_type bad_input;
                std::getline(m_is, bad_input);
                m_on_failure(bad_input);

                //put m_is on a good state
                m_is.clear();
                m_is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                output_object<CharT>::object << m_on_failure_msg << std::endl;
            }
        }
        return *this;
    }

    /**
     * @brief bound_input
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] min_delimiter : minimum bound.
     * @param[in] max_delimiter : minimum bound.
     * @param[in] oob_message : message to display if the value entered is out of bound.
     * @param[in] comp : Binary predicate that takes two arguments as user_var which return a bool. The expression comp(a,b), where comp is an object of this type and a and b are key values, shall return true if a is considered to go before b in the strict weak ordering the function defines. The default value will be less<Arithmetic_type>.
     *
     * @pre "user_var" has to implement an operator>>(istream&, var_type). Arithmetic_Type has to be arithmetic. "comp" has to return a bool or a value convertible into a bool and take two arguments.
     *
     * @return sis
     *
     * Same behaviour as operator>>(istream&, ...) but guarantee than at end of func, user_var will be in a right state AND not out of specified bound.
     */
    template <typename Arithmetic_Type, typename Comparator>
    basic_sistream& bound_input(Arithmetic_Type& user_var, Arithmetic_Type min_delimiter = std::numeric_limits<Arithmetic_Type>::min(), Arithmetic_Type max_delimiter = std::numeric_limits<Arithmetic_Type>::max(), string_type const& oob_message = "", Comparator && comp = std::less<Arithmetic_Type>())
    {
        static_assert(std::is_arithmetic<Arithmetic_Type>::value, "Error in sistream operator<< with Arithmetic_Type which is not arithmetic.");
        do
        {
            *this >> user_var;
            if (comp(user_var, min_delimiter) || comp(max_delimiter, user_var))
                output_object<CharT>::object << (oob_message == "" ? m_on_failure_msg : oob_message) << std::endl;
        } while (comp(user_var, min_delimiter) || comp(max_delimiter, user_var));
        return *this;
    }

    /**
     * @brief predicate_input
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] pred : any callable object which returns bool and can take one argument convertible to Input_Type&&. It has to return true if the value is considered as an acceptable value.
     *
     * @pre : Predicate as to be an unary predicat which returns a bool value.
     *
     * Input function which force the user to enter a value which match with a predicate.
     *
     */
    template <typename Input_Type, typename Predicate>
    basic_sistream& predicate_input(Input_Type& user_var, Predicate && pred)
    {
        static_assert(std::is_convertible<Predicate, std::function<bool(Input_Type&&)>>::value, "Error in sistream predicate : Predicate is not convertible to std::function<bool(Input_Type)>.");
        do
        {
            *this >> user_var;
        } while (!pred(user_var));
        return *this;
    }

    /**
     * @brief any_of_container
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] first : first iterator of a sequence.
     * @param[in] last : last iterator of a sequence.
     *
     * Input function which force user to enter a value within a container.
     *
     */
    template <typename Input_Type, typename Iterator>
    basic_sistream& any_of_container(Input_Type& user_var, Iterator first, Iterator last)
    {
        return predicate_input(user_var, [&](Input_Type& val){return (std::find(first, last, val) != last);});
    }

    /**
     * @brief any_of
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] args : set of values which contains every possible "good" values.
     *
     * @pre set of value has to be convertible to an unique type.
     *
     * Input function which force user to enter a value within a set of values. Same as any_of_container but with variadic template.
     *
     */
    template <typename Input_Type, typename... Args>
    basic_sistream& any_of(Input_Type& user_var, Args&&... args)
    {
        auto array = make_array(args...);
        return any_of_container(user_var, std::begin(array), std::end(array));
    }

    /**
     * @brief none_of_container
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] first : first iterator of a sequence.
     * @param[in] last : last iterator of a sequence.
     *
     * Input function which force user to enter a value which is not inside a container.
     *
     */
    template <typename Input_Type, typename Iterator>
    basic_sistream& none_of_container(Input_Type& user_var, Iterator first, Iterator last)
    {
        return predicate_input(user_var, [&](Input_Type& val){return (std::find(first, last, val) == last);});
    }

    /**
     * @brief none_of
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] args : set of values which contains every possible "bad" values.
     *
     * @pre set of value has to be convertible to an unique type.
     *
     * Input function which force user to enter a value which is not inside a set of values. Same as none_of_container but with variadic template.
     *
     */
    template <typename Input_Type, typename... Args>
    basic_sistream& none_of(Input_Type& user_var, Args&&... args)
    {
        auto array = make_array(args...);
        return none_of_container(user_var, std::begin(array), std::end(array));
    }

    /**
     * @brief default move constructor
     */
    basic_sistream(basic_sistream && other) = default;

    basic_sistream& operator=(basic_sistream && other) = default;

private:
    basic_sistream(basic_sistream const& other) = delete;
    basic_sistream& operator=(basic_sistream const& other) = delete;

    istream_type& m_is;
    string_type m_on_failure_msg;
    Func m_on_failure;
};

// typedef(s) :

typedef basic_sistream<char, std::function<void(output_object<char>::type)>, std::char_traits<char>> sistream;
typedef basic_sistream<wchar_t, std::function<void(output_object<wchar_t>::type)>, std::char_traits<wchar_t>> wsistream;
template <typename Func> using custom_sistream =  basic_sistream<char, Func, std::char_traits<char>>;
template <typename Func> using custom_wsistream =  basic_sistream<wchar_t, Func, std::char_traits<wchar_t>>;

/**
 * @brief scin global variable
 *
 * Equivalent of cin.
 *
 */
extern sistream scin;

/**
 * @brief swcin global variable
 *
 * Equivalent of wcin.
 *
 */
extern wsistream swcin;


#endif // SIOSTREAMMM
