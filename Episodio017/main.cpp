#include "tronco.h"

int main(int argc, char* /*argv*/[])
{
     LOG_INFO << "Comenzando.";

     LOG_INFO << "Estoy en: " << __FUNCTION__;

     LOG_INFO_FMT("Me pasaron {} argumentos.", (argc - 1));

     return 0;
}
