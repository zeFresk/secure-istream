// Secure input header.
// It contains the definition of sostream, which is a secured version of istream.
// This class is a high level class so it will not provide much functions.
// I know this could have been better but if you want to code a better version feel free to do so.

// This code is under MIT license.
// by zeFresk, 2015

#ifndef SIOSTREAM
#define SIOSTREAM

#include <iostream>
#include <stdexcept>
#include <limits>
#include <type_traits>

/**
 * @brief is_istream
 *
 * Template trait class which return whereas template parameter is an std::istream or not
 */
template <typename>
struct is_istream : std::false_type {};

template <typename CharT, typename Traits>
struct is_istream<basic_istream<CharT, Traits>> : std::true_type {};


/**
 * @class sistream
 * @brief secure istream class.
 *
 * High level version of istream which provide strong garantee and force user to enter a correct input.
 * It was made to be as easy to use as possible.
 */
template <typename Func, typename Func2>
class sistream
{
public:

    /**
     * @brief constructor
     * @param[in] is : input stream.
     * @param[in] error_message : text which is going to be outputted if the user's input isn't valid. (std::cout)
     * @param[in] on_failure : Func to call on user's bad input. Can take ONLY ONE parameter which has to be a std::string which whill contains user input. Return of on_failure will be ignored. Func can be any callable thing.
     *
     * @pre "is" has to be a console stream such as cin. "errror_message" can't be empty.
     *
     */
    sistream(std::istream is, std::string error_message = "Please enter a correct value.", Func on_failure = [](void){}) : m_is(is), m_on_failure_msg(error_message), m_on_failure(on_failure)
    {
        static_assert(is_istream<is>::value, "Error in sistream ctor : \"is\" has to be a std::istream.");
        assert(!error_message.empty() && "Error in sistream ctor : \"error_message\" is empty");
    }

    /**
     * @brief raw_istream
     * @return encapsuled std::istream
     */
    std::istream& raw_istream() const {return m_is;}

    /**
     * @brief operator >>
     * @param[in] sis : secure input stream.
     * @param[out] user_var : variable which is going to contains user's good input.
     *
     * @pre user_var has to implement an operator>>(istream&, var_type).
     *
     * @return sis
     *
     * Same behaviour as operator>>(istream&, ...) but guarantee than at end of func, user_var will be in a right state.
     */
    template <typename Input_Type>
    sistream& operator>>(Input_Type const& user_var)
    {
        while (!(m_is >> user_var))
        {
            if (m_is.eof()) //End-Of-File
            {
                throw std::runtime_error("Error in operator>>(sistream&...) : EOF.");
            }
            else if (m_is.bad()) //Bad bit
            {
                throw std::runtime_error("Error in operatro>>(sistream&...) : Bad bit.");
            }
            else //bad user's input
            {
                //backup the bad user's input
                std::string bad_input;
                std::getline(m_is, bad_input);
                m_on_failure(bad_input);

                //put m_is on a good state
                m_is.clear();
                m_is.ignore( numeric_limits<streamsize>::max(), '\n');

                std::cout << m_on_failure_msg << std::endl;
            }
        }
        return *this;
    }

    /**
     * @brief getline
     * @param[in] sis : secure input stream.
     * @param[out] user_var : variable which is going to contains user's good input.
     * @param[in] delim : delimiter char.
     *
     * @pre user_var has to implement getline(istream&, var_type).
     *
     * @return sis
     *
     * @todo implement.
     *
     * @deprecated not implemlented...
     *
     * Same behaviour as getline but guarantee than user_var will be in a good state.
     * (Not implemented yet)
     */
    template <typename Input_type, typename CharT>
    friend sistream& getline(sistream& sis, Input_Type const& user_var);
private:
    std::istream& m_is;
    std::string m_on_failure_msg;
    Func m_on_failure;
};

#endif // SIOSTREAM

