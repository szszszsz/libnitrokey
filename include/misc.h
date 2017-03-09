#ifndef MISC_H
#define MISC_H
#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include "log.h"
#include "LibraryException.h"

namespace nitrokey {
namespace misc {

    template <typename T>
    void strcpyT(T& dest, const char* src){

        if (src == nullptr)
//            throw EmptySourceStringException(slot_number);
            return;
        const size_t s_dest = sizeof dest;
        nitrokey::log::Log::instance()(std::string("strcpyT sizes dest src ")
                                       +std::to_string(s_dest)+ " "
                                       +std::to_string(strlen(src))+ " "
            ,nitrokey::log::Loglevel::DEBUG);
        if (strlen(src) > s_dest){
            throw TooLongStringException(strlen(src), s_dest, src);
        }
        strncpy((char*) &dest, src, s_dest);
    }


    template <typename T>
typename T::CommandPayload get_payload(){
    //Create, initialize and return by value command payload
    typename T::CommandPayload st;
    bzero(&st, sizeof(st));
    return st;
}

    template<typename CMDTYPE, typename Tdev>
    void execute_password_command(Tdev &stick, const char *password) {
        auto p = get_payload<CMDTYPE>();
        p.set_defaults();
        strcpyT(p.password, password);
        CMDTYPE::CommandTransaction::run(stick, p);
    }

    std::string hexdump(const char *p, size_t size, bool print_header=true, bool print_ascii=true,
        bool print_empty=true);
    uint32_t stm_crc32(const uint8_t *data, size_t size);
    std::vector<uint8_t> hex_string_to_byte(const char* hexString);
}
}

#endif
