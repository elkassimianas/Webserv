#ifndef PARSINGREQUEST_HPP
#define PARSINGREQUEST_HPP

#include <string>
#include <map>
#include <iostream>

enum HTTPfields
{
   //Authorization,
    Content_Length,
    Content_Type,
    Host,
    Connection
};


class ParsingRequest
{
    protected:
        std::string                 method;
        char                        *URI;
        std::map<int, std::string>  errors;
        std::map<int, std::string>  fields;
        std::string                 request;
    public:
        ParsingRequest();
        ~ParsingRequest();

        void        end_header_fields(char **request);
        void        request_line(char *request_line);
        void        header_fields(char *header_fields);
        void        set_errors(void);
        std::string get_fields(int field);
        std::string get_request(void);
        void        set_request(std::string request);
        void        start_parsing(void);
        ParsingRequest& operator = (ParsingRequest const &obj);
};

#endif