#include <string>
#include <set>
#include <stack>
#include <ostream>
#include <iomanip> 

class dendl_t{ };
class ddec_t{ };
class dhex_t{ };

/*!
 * \brief class supporting switchable debug output
 *
 * This class supplies another output stream called "dout",
 * which can be configurated to print some output but not all of them.
 * This is useful for debug output.
 *
 * Advantage over debug macros is that it can be configured
 * via files and no recompile is necessary.
 *
 * It is configured via "scopes". For it to work correctly, 
 * the programmer has to mark start and end of scopes in the program.
 * Display of entering and exiting scopes can be switched on and off
 * while in the program but more often a configuration file is used.
 *
 * The Configuration File should be loaded at the beginning of the program.
 * It consists of lines in which each line is one debug "scope". To
 * deactivate a scope in the configuration file, prepend "//" (without quotes)
 *
 * In the program itself, startScope is used to start a new scope
 * whereas endScope is used to mark the end of one.
 * In this scope, additional debug output can be written using
 * dout << "something" (like with cout). This output will only be written
 * if the current scope is "active"
 *
 * \note The programmer is responsible to ensure that scopes are 
 *       ended when they really are. Beware that functions can be
 *       early-terminated by return and loops may be BREAKed or CONTINUEd
 */
class dout_t{
  public:
    /*!
     * \brief activate all scopes that are in the given file
     *
     * This function opens the given config file and
     * reads it line by line. Each line that does not begin
     * with "//" will be regarded as a scope activation
     *
     * \param[in] filename name of the config file
     */
    void loadConfigFile(const std::string& filename);
    /*!
     * \brief start a scope
     *
     * This function starts a scope, effectively
     * putting the name on the stack and checking whether
     * this scope is active. If it is, a short message
     * is printed
     *
     * \param[in] scopeName   name of the starting scope
     */
    void startScope(const std::string& scopeName);
    /*!
     * \brief ends a scope
     *
     * This function ends a scope. If this scope
     * was active, a short message is printed.
     * Afterwards, the name is removed from the stack and it is checked,
     * whether the next scope on the stack is active.
     *
     * \note Ending a scope does not need a scopeName. If you supply
     *       one, the name of the ended scope is checked against
     *       the top one on stack and an error is printed if they
     *       don't match. This can help finding non-ended scopes
     *
     * \param[in] scopeName  name of the ending scope
     */
    void endScope(const std::string& scopeName);
    /*!
     * \brief ends current scope
     *
     * This function ends the current scope. If this scope
     * was active, a short message is printed. Afterwards,
     * the name is removed from the stack and it is checked whether the next
     * scope on the stack is active.
     *
     * \note Ending a scope does not need a scopeName. You may
     *       supply one for more checking (see function with parameter)
     */
    void endScope();
    
    /*!
     * \brief querys whether scope is active
     *
     * This function querys whether the given scope is
     * active.
     *
     * \param[in] scopeName   name of the scope
     * \return                true, if scope has been actived. false, if not
     */
    bool isActivated(const std::string& scopeName)const;
    
    /*!
     * \brief Activate or deactivate a scope
     *
     * This function activates or deactivates a scope so that entering, exiting and
     * conditional output will be printed (or not).
     *
     * This is useful for activating or deactivating something manually (mainly debugdebug),
     * but should also be used as main function for activating and deactivating scopes
     *
     * \param[in] scopeName   name of the scope
     * \param[in] activate    true: activate, false: deactivate
     */
    void setScopeActive(const std::string& scopeName, const bool activate);
    
    //! Wrapper for setScopeActive(... , true)
    void activateScope(const std::string& scopeName);
    
    //! Wrapper for setScopeActive(... , false)
    void deactivateScope(const std::string& scopeName);

    /*!
     * \brief write something to cout if current scope is active
     *
     * This function writes the given string to stdoutput if 
     * the current scope is active
     *
     * \todo this is a preliminary to dout << something
     *
     * \param[in] debugOutput String to print
     */
    void dout(const std::string& debugOutput)const;
    /*!
     * \brief print current scope stack
     *
     * This function prints the stack of currently open scopes
     * to the given output stream (e.g. cout)
     *
     * Note that even not-activated-scopes are on this stack.
     *
     * \param outStream   stream to write the information to
     */
    void printStack(std::ostream& outStream)const;
    /*!
     * \brief print activated scopes
     *
     * This function prints a list of all activated scopes
     * to the given output stream (e.g. cout)
     *
     * It is merely used for debug purposes
     *
     * \param outStream   stream to write the information to
     */
    void printActivatedScopes(std::ostream& outStream)const;
    
    bool currentActive;

    //! ReDefining << on strings (chararrays)
    dout_t& operator<< (const char* output );
    //! ReDefining << on strings (real strings)
    dout_t& operator<< (const std::string output);
    //! ReDefining << on integers
    dout_t& operator<< (const int output);
    
    //! ReDefining << on ostream-functions (like std::endl)
    dout_t& operator<< (std::ostream& ( *functionPointer )(std::ostream&));
    
    //! ReDefining << on ios_base-functions (like std::dec)
    dout_t& operator<< (std::ios_base& ( *functionPointer)(std::ios_base&));

    //! ReDefining << on _Setw-functions (needed for std::setw(something))
    dout_t& operator<<(std::_Setw (functionPointer));
    
    //! ReDefining << on _SetFill-Functions (needed for std::setfill(something))
    dout_t& operator<<(std::_Setfill<char> (functionPointer));
    //dout_t& operator<< (ios& ( *pf )(ios&));
    
  private:
    std::set < std::string > activatedScopes; //!< set of activated scopes
    std::stack < std::string > currentScopeStack; //! stack of current scopes
  
};


extern dout_t  dout;

