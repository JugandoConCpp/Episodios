#include <tronco.h>

Tronco::Tronco Main_Logger;

int main(int argc, char* /*argv*/[])
{
     Main_Logger.Add_Channel<Tronco::File_Channel>("log_tester");
     Main_Logger.Add_Channel<Tronco::Console_Channel>("Console");

     LOG_INFO << "Comenzando.";

     LOG_INFO << "Estoy en: " << __FUNCTION__;

     LOG_INFO_FMT("Me pasaron {} argumentos.", (argc - 1));

     return 0;
}
