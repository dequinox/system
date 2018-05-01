#ifndef EXCEPTION_H
#define EXCEPTION_H

class GraphException: public std::exception
{
      public:
            virtual const char* what() const throw()
            {
                  return "Syntax error";
            }
};

#endif /* EXCEPTION_H */
