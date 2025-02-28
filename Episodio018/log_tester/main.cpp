#include <tronco.h>

Tronco::Tronco Main_Logger;

int main(int argc, char* /*argv*/[])
{
     Main_Logger.Add_Channel(std::make_unique<Tronco::Console_Channel>("Consola"));

     LOG_INFO << "Comenzando.";

     LOG_INFO << "Estoy en: " << __FUNCTION__;

     LOG_INFO_FMT("Me pasaron {} argumentos.", (argc - 1));

     return 0;
}
