#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H
#include <exception>


class GameException: public std::exception
{
    const char* const message;

    public:
        GameException(const char * const s):message(s){}
        const char* what() const throw() { return message; }
        virtual ~GameException() throw() {}
};

#endif // GAMEEXCEPTION_H
